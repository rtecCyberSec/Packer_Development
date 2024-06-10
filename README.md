# Offensive Packer Development
![Language - Nim](https://img.shields.io/badge/Language-Nim-2ea44f) ![Language - Rust](https://img.shields.io/badge/Language-Rust-2ea44f) ![Language - C](https://img.shields.io/badge/Language-C-2ea44f)

This repository was created in conjunction with a Packer Development Workshop held at [x33fcon](https://www.x33fcon.com/#!index.md) 2024 by [S3cur3Th1sSh1t](https://github.com/s3cur3th1ssh1t) and [eversinc33](https://github.com/eversinc33). It contains the Slides of the presentation plus code snippets in different programming languages for offensive Packer Development. 

<br>
<div align="center">
    <img src="https://github.com/rtecCyberSec/Packer-Development/assets/27858067/1ce772fd-92b2-4dea-8c0c-30d77a2b76a2" width="500">
</div>
<br>

The goal of the workshop is to show participants the concept of packed malware. How does a Packer work technically? Using public toolings without modifications from Github (including this repo's examples ;-)) will nowadays lead to Red Teams or Threat Actors getting caught real quick. To avoid detections, it's therefore needed to build custom tooling and/or to learn about Malware Development in general. Automating the process of taking an input payload, encrypting it, building loader code and compiling that afterward saves a lot of time for operators. 

How does a Packer work technically? How to load/execute C# assemblies, PE's or Shellcode from memory on runtime? How to get rid of entropy based detections? Which evasion techniques should be integrated? How about Sandbox evasion and Anti-Debugging techniques or environmental keying? Answers to these questions are included within this repository.

### Getting started

You can choose a guideline in one of the following languages to get started building your packer:

- [Nim](./Nim)
- [Rust](./Rust)
- [C](./C)
