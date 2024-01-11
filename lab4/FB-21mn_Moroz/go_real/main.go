package main

import (
	"crypto"
	"crypto/rand"
	"crypto/rsa"
	"crypto/sha256"
	"encoding/base64"
	"encoding/json"
	"fmt"
	"io/ioutil"
    "golang.org/x/crypto/ssh"
	"net/http"
	"os"
)

var privateKey *rsa.PrivateKey

func init() {
	// Load private key from file
	keyBytes, err := ioutil.ReadFile("private_key.pem")
	if err != nil {
		fmt.Println("Failed to read private key file:", err)
		os.Exit(1)
	}

	privateKey, err = parseRSAPrivateKey(keyBytes)
	if err != nil {
		fmt.Println("Failed to parse private key:", err)
		os.Exit(1)
	}
}

func main() {
	http.HandleFunc("/sign", signHandler)
	http.HandleFunc("/verify", verifyHandler)

	fmt.Println("Server is running on :8080...")
	err := http.ListenAndServe(":8080", nil)
	if err != nil {
		fmt.Println("Error starting the server:", err)
	}
}

func signHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	var requestData map[string]string
	err := json.NewDecoder(r.Body).Decode(&requestData)
	if err != nil {
		http.Error(w, "Invalid JSON data", http.StatusBadRequest)
		return
	}

	data, ok := requestData["data"]
	if !ok {
		http.Error(w, "Missing 'data' field in JSON", http.StatusBadRequest)
		return
	}

	signature, err := signData([]byte(data))
	if err != nil {
		http.Error(w, "Failed to sign data", http.StatusInternalServerError)
		return
	}

	response := map[string]string{"signature": base64.StdEncoding.EncodeToString(signature)}
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(response)
}

func verifyHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
		return
	}

	var requestData map[string]string
	err := json.NewDecoder(r.Body).Decode(&requestData)
	if err != nil {
		http.Error(w, "Invalid JSON data", http.StatusBadRequest)
		return
	}

	data, ok := requestData["data"]
	if !ok {
		http.Error(w, "Missing 'data' field in JSON", http.StatusBadRequest)
		return
	}

	signatureBase64, ok := requestData["signature"]
	if !ok {
		http.Error(w, "Missing 'signature' field in JSON", http.StatusBadRequest)
		return
	}

	signature, err := base64.StdEncoding.DecodeString(signatureBase64)
	if err != nil {
		http.Error(w, "Invalid base64-encoded signature", http.StatusBadRequest)
		return
	}

	valid := verifySignature([]byte(data), signature)
	response := map[string]bool{"valid": valid}

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(response)
}

func signData(data []byte) ([]byte, error) {
	hashed := sha256.Sum256(data)
	signature, err := rsa.SignPKCS1v15(rand.Reader, privateKey, crypto.SHA256, hashed[:])
	if err != nil {
		return nil, err
	}
	return signature, nil
}

func verifySignature(data, signature []byte) bool {
	hashed := sha256.Sum256(data)
	err := rsa.VerifyPKCS1v15(&privateKey.PublicKey, crypto.SHA256, hashed[:], signature)
	return err == nil
}

func parseRSAPrivateKey(keyBytes []byte) (*rsa.PrivateKey, error) {
	// Try to parse as PKCS1 private key

	key, err := ssh.ParseRawPrivateKey(keyBytes)

    return key.(*rsa.PrivateKey), err


	// privateKey, err := rsa.ParsePKCS1PrivateKey(keyBytes)
	// if err == nil {
	// 	return privateKey, nil
	// }

	// // If parsing as PKCS1 fails, try to parse as PKCS8 private key
	// privateKey, err = parsePKCS8PrivateKey(keyBytes)
	// if err != nil {
	// 	return nil, err
	// }

	return privateKey, nil
}

func parsePKCS8PrivateKey(keyBytes []byte) (*rsa.PrivateKey, error) {
	// Implement parsing PKCS8 private key here
	// You may use third-party libraries like "github.com/golang/crypto"
	// to parse PKCS8 private key
	// Example: https://pkg.go.dev/golang.org/x/crypto#example-ParsePKCS8PrivateKey
	return nil, fmt.Errorf("PKCS8 private key parsing not implemented")
}
