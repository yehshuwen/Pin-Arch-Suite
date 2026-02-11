# Pin-Arch-Suite

This project is developed using the **Intel Pin Dynamic Binary Instrumentation (DBI)** framework. It serves as a collection of architectural analysis tools designed to profile the runtime characteristics of x86-64 applications.

---

## 📌 Tool Spotlight: Dependency Analyzer

The first core tool in this suite is a profiler for instruction-level **Read-After-Write (RAW) register dependency distances**.

* **Analysis Logic**: It tracks the number of instructions executed between a register write and its subsequent read to measure data dependency patterns.
* **Technical Implementation**: The tool provides comprehensive support for the x86-64 register set.
* **Precision**: It accurately maps partial register accesses (e.g., `%al`, `%ax`) to their respective architectural registers (e.g., `%rax`) to ensure consistent dependency tracking and avoid undercounting.

---

## 🚀 Getting Started

### 1. Prerequisites
Before using this tool, you must download the Intel Pin toolkit (version 3.x recommended) from the [official Intel website](https://www.intel.com/content/www/us/en/developer/articles/tool/pin-a-dynamic-binary-instrumentation-tool.html).

### 2. Building the Tool
Ensure the `PIN_ROOT` environment variable is set to your Pin installation directory.
```bash
cd tools/dependency-analyzer/src
make
