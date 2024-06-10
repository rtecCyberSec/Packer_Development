use std::env;
use std::fs::File;
use std::io::Read;
use std::io::Write;

fn encrypt_decrypt (data: Vec<u8>) -> Vec<u8> {
    let key: [u8; 4] = [0x11, 0x22, 0x33, 0x44];
    let mut decrypted_data = data.clone();
    for i in 0..data.len() {
        let is_divisible = data.len() % 4 == 0;
        let rest = data.len() % 4;
        if is_divisible {
            decrypted_data[i] ^= key[i % 4];
        } else {
            if i < data.len() - rest {
                decrypted_data[i] ^= key[i % 4];
            } else {
                decrypted_data[i] ^= key[0];
            }
        }
    }
    decrypted_data
}
