# 🏦 Bank Account Management System

A command-line C program for creating and managing checking and savings 
bank accounts with dynamic memory allocation and multi-field sorting.

## Features
- Create checking (C) and savings (S) accounts with validation
- Deposit and withdraw funds with balance verification
- Sort accounts by account number, holder name, open date, or balance
- Dynamic memory allocation with `realloc` for unlimited accounts
- Input validation for account numbers, dates (YYYY-MM-DD), and amounts
- Duplicate account detection

## How to Run
```bash
git clone https://github.com/melainefernandez/bank-account-management.git
cd bank-account-management
gcc bank_account.c -o bank_account
./bank_account
```

## Menu Options
| Code | Action |
|------|--------|
| `c` | Create a new account |
| `d` | Deposit money |
| `w` | Withdraw money |
| `s` | Sort and print all accounts |
| `q` | Quit |

## Tech Stack
`C` `GCC` `Dynamic Memory Allocation` `qsort`
