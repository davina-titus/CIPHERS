# Ciphers — Encryption & Decryption 🔐

A C++ program that implements multiple classical cipher algorithms 
for encrypting and decrypting text, exploring the foundations of 
cryptography through hands-on implementation.

## Overview
Before modern encryption, classical ciphers were the backbone of 
secret communication. This project implements several of the most 
historically significant cipher algorithms from scratch in C++, 
covering both substitution and transposition techniques.

## Ciphers Implemented
- **Caesar Cipher** — shifts each letter by a fixed number of positions
- **Vigenère Cipher** — uses a keyword to apply variable letter shifts
- **ROT13** — a special case Caesar cipher with a shift of 13
- **Atbash Cipher** — reverses the alphabet (A↔Z, B↔Y, etc.)

> *Note: See source code for full list of implemented ciphers*

## Features
- Encrypt and decrypt text using any supported cipher
- Handles both uppercase and lowercase input
- Preserves spaces, punctuation, and non-alphabetic characters
- Menu-driven interface for selecting cipher and operation
- Makefile for streamlined compilation

## Example
```
Input:  "Hello, World!"
Cipher: Caesar (shift 3)
Output: "Khoor, Zruog!"

Decrypt:
Input:  "Khoor, Zruog!"
Output: "Hello, World!"
```

## Concepts Demonstrated
- String manipulation and character arithmetic in C++
- Modular arithmetic for wrapping cipher shifts
- Encryption and decryption symmetry
- Menu-driven program architecture
- Build automation with Makefile
- Introduction to cryptographic thinking

## How to Run
```bash
git clone https://github.com/davina-titus/CIPHERS.git
cd CIPHERS
make
./ciphers
```

## Language
- **C++** with Makefile build system

---
