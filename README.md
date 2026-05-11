# operating-systems-voter-mgmt

A **command-driven CLI tool for managing election state** in C — fast lookup of voters by national ID (PIN), fast aggregation of votes-cast by postal code, save/restore to file, recovery from `malloc` failure mid-operation. Built around a generic hash-table module and a `ListOfLists` data structure for the per-zip-code views.

Class project for the **Operating Systems** course at the University of Athens (Department of Informatics & Telecommunications). Solo project. *Earlier-year attempt — see the [Class history](#class-history) note at the end.*

## What it does

```
                ┌───────────────────────────────┐
                │            mvote              │
                │   (REPL: 13 commands)         │
                └──────────────┬────────────────┘
                               │
                ┌──────────────┴──────────────┐
                ▼                             ▼
       ┌─────────────────┐          ┌─────────────────────┐
       │   HashTable     │          │     ListOfLists     │
       │  PIN → Voter    │          │  zip → list of      │
       │  linear probing │          │       voters who    │
       │  generic over   │          │       have voted    │
       │  Item, Key      │          │  (kept ordered)     │
       └─────────────────┘          └─────────────────────┘
```

The `i` command inserts into the hash table; `m` (mark) flips a voter's status to "has voted" and inserts the voter pointer into the `ListOfLists` keyed by their zip code. Reads (`v`, `perc`, `z`, `o`) hit only the structure that makes them O(1) or O(zip-bucket-size).

```
$ ./mvote -f test_files/voters5000.csv -i 16 -b 2
Enter your command: i 12345678 Doe John 10434
Enter your command: m 12345678
Enter your command: z 10434
Enter your command: o
Enter your command: perc
Enter your command: save snapshot.csv
Enter your command: exit
```

Flags: `-f` loads initial voter set; `-i` initial bucket count; `-b` entries per bucket.

## What was interesting

- **Generic hash-table module** (`modules/hash_table.{c,h}`). The header defines `typedef Voter Item`, `typedef int Key`, and accepts a key→int function plus a destroy-item function as parameters — so the same module could swap to any other domain by changing the typedefs.
- **Malloc-failure recovery as a design constraint.** Each module is written so that an OOM mid-operation rolls back partial work, frees temporaries, and returns an error without corrupting structure. Worth it because the program is meant to keep running after OOM: the user can free memory elsewhere, then `save` and exit cleanly.
- **Command dispatch via static struct array.** `commands[]` in `commandHandler.c` maps name → function pointer. Adding a new command is one new `command_<name>.c` file plus one row in the array — keeps the entry points discoverable and the dispatch loop dead-simple.
- **Per-zip list-of-lists kept ordered.** Inserting a voter into a zip-code bucket maintains zip-code order across the outer list, so the `o` command (most→least zips by votes cast) just walks once instead of sorting on every query.

## Commands

| Command | What it does |
|---|---|
| `l <PIN>` | Print voter by PIN |
| `i <PIN> <lname> <fname> <zip>` | Insert new voter |
| `if <file>` | Insert voters from CSV |
| `m <PIN>` | Mark voter as having voted |
| `bv <file>` | Bulk-mark votes from PIN list file |
| `v` | Count voters who voted |
| `perc` | Percentage who voted |
| `z <zip>` | PINs of voters who voted in a given zip code |
| `o` | Order zips by vote count |
| `save <file>` | Snapshot current state |
| `clear` / `help` / `exit` | UI |

## Build & run

```bash
make
./mvote -f test_files/voters5000.csv
```

Test data ships in `test_files/`:
- `voters{50,5000}.csv` — initial voter sets
- `keys{50,5000}.csv` — PIN lists for `bv` bulk-marking
- `save{50,5000}.csv` — snapshots for `-f` round-trip testing

## Modules

| File | Purpose |
|---|---|
| `mvote.c` | Entry point: arg parsing, initial-file load, REPL |
| `commandHandler.c` | Command-name → function dispatch table |
| `commands/command_*.c` | One file per command — implementation is local to its file |
| `modules/hash_table.c` | Generic linear-probing hash table with malloc-failure rollback |
| `modules/ListOfLists.c` | Outer list of zip codes, each holding an ordered list of voters |
| `modules/voter.c` | The `Voter` entity itself |

## Class history

This is from the **earlier year** I took the Operating Systems course. The polished follow-up — a different homework from the next year's retake — is at [`operating-systems-hw1`](https://github.com/AlexTuring010/operating-systems-hw1) (a shared-memory + POSIX-semaphores piece). The other older-year piece is the [`operating-systems-parallel-sort`](https://github.com/AlexTuring010/operating-systems-parallel-sort) fork+pipes assignment. All three are kept public deliberately.

## License

[MIT](LICENSE) — applies to my own code in this repo. Assignment-distributed materials retain their original course copyright.
