# House of Delegates

Some CTF pwnable challenges based on a custom heap allocator.

### To build the challenges:
1. run `cargo build` from the root directory (intentionally using the default debug mode, as we want to provide symbols).
2. `cd` to the `c_src` directory and run `make`.

### Requirements:
- a recent version of `rust stable`
- some C compiler (modify `c_src/Makefile` if you want to change away from `gcc`)
- `rm`
- `ls`

### On Allocator Design

`libgarntalloc.so` implements a bump allocator that uses a freelist to prioritize allocation reuse. No metadata whatsoever is stored inline.

`gmalloc()` allocation process:
1. Bail if the size requested bigger than one system page (4096 bytes)
2. Attempt to re-use an allocation from the freelist in one of the already-reserved pages
3. If that didn't work, attempt to use the bump allocator to allocate space within one of the already-reserved pages
4. If that didn't work, `mmap()` to reserve a new page and then use the bump allocator in the fresh page

`gfree()` checks if the address is allocated in one of the currently-allocated pages, and if it is, removes it from the metadata and adds it to the free list.

### On the Etymology of "House of Delegates"

*Shame on you if you don't know what this is, civil engagement is important.*

The [Maryland House of Delegates](https://en.wikipedia.org/wiki/Maryland_House_of_Delegates) is the lower house of the legislature of the State of Maryland.

In Computer Science, [Delegation](https://en.wikipedia.org/wiki/Delegation_(object-oriented_programming)) is an OOP design pattern that frequently results in the use of function pointers and vtables internally.