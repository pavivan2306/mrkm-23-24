from flask import Flask, request, jsonify
from Crypto.Hash import SHA256
from Crypto.PublicKey import RSA
from Crypto.Signature import PKCS1_PSS
from Crypto.Util import asn1
import base64

# Load keys from files
with open("private_key.pem", "rb") as key_file:
    private_key = RSA.import_key(key_file.read())

with open("public_key.pem", "rb") as key_file:
    public_key = RSA.import_key(key_file.read())

app = Flask(__name__)

@app.route("/v1/sign", methods=["POST"])
def sign_data():
    data = request.json.get("data")  # Accept any data format
    if not data:
        return jsonify({"error": "Missing data to sign"}), 400

    try:
        data_bytes = base64.b64decode(data)
    except (base64.binascii.Error, TypeError):
        return jsonify({"error": "Invalid data format"}), 400

    hash_obj = SHA256.new(data_bytes)
    signer = PKCS1_PSS.new(private_key)
    signature = signer.sign(hash_obj)

    return jsonify({"signature": base64.b64encode(signature).decode()})


@app.route("/v1/verify", methods=["POST"])
def verify_signature():
    data = request.json.get("data")
    signature = request.json.get("signature")
    if not data or not signature:
        return jsonify({"error": "Missing data or signature"}), 400

    try:
        data_bytes = base64.b64decode(data)
        sig_bytes = base64.b64decode(signature)
    except (base64.binascii.Error, TypeError):
        return jsonify({"error": "Invalid data format"}), 400

    hash_obj = SHA256.new(data_bytes)
    verifier = PKCS1_PSS.new(public_key)

    if verifier.verify(hash_obj, sig_bytes):
        return jsonify({"verified": True})
    else:
        return jsonify({"verified": False})


if __name__ == "__main__":
    app.run(debug=True, ssl_context="adhoc")
