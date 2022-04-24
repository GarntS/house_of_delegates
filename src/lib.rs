/* file:    lib.rs
 * author:  garnt
 * date:    04/22/2022
 * desc:    Lazy malloc implementation
 */

use lazy_static::lazy_static;
use memmap2::MmapMut;
use std::collections::{HashMap, VecDeque};
use std::sync::Mutex;
use std::vec::Vec;

// constant defining the page size on 
const PAGE_SIZE: usize = 4096;

// struct to represent a single allocation
struct Mallocation {
    addr: usize,
    size: usize,
}

// struct to store all of the information required to keep track of the global
// allocator state
struct PageInfo {
    _page: MmapMut,
    page_addr: usize,
    page_size: usize,
    bump_addr: usize,
    available_bump_size: usize,
    n_allocations: usize,
    alloc_sizes: HashMap<usize, usize>,
    free_list: VecDeque<Mallocation>
}

// global vec of allocated pages
lazy_static! {
    static ref ALLOCATED_PAGES: Mutex<Vec<PageInfo>> = Mutex::new(Vec::new());
}

// allocate_page() allocates a page of memory to be used by the allocator
fn allocate_page() -> MmapMut {
    return MmapMut::map_anon(PAGE_SIZE).unwrap();
}

// has_reusable_allocation() returns true if there exists a reusable allocation
// of the requested size in page
fn has_reusable_allocation(page: &PageInfo, size: usize) -> bool {
    page.free_list.iter().find(|block| block.size >= size).is_some()
}

// has_bump_space() returns true if there is enough space remaining to be used
// by the bump allocator within page
fn has_bump_space(page: &PageInfo, size: usize) -> bool {
    page.available_bump_size >= size
}

// reuse_alloc() reserves a new bump-allocated allocation of the requested size
fn reuse_alloc(page: &mut PageInfo, size: usize) -> usize {
    let index: usize = page.free_list.iter().position(|block| block.size >= size).unwrap();
    let block: Mallocation = page.free_list.remove(index).unwrap();
    page.alloc_sizes.insert(block.addr, size);
    block.addr
}

// reuse_alloc() reserves a new bump-allocated allocation of the requested size
fn bump_alloc(page: &mut PageInfo, size: usize) -> usize {
    let alloc_addr: usize = page.bump_addr;
    page.bump_addr += size;
    page.available_bump_size -= size;
    page.n_allocations += 1;
    page.alloc_sizes.insert(alloc_addr, size);
    alloc_addr
}

// malloc() allocates a block of memory of the requested size
#[no_mangle]
pub extern "C" fn gmalloc(size: usize) -> usize {
    // make sure we aren't trying to allocate more than a page
    if size > PAGE_SIZE {
        return 0;
    }

    // lock the mutex
    let pages: &mut Vec<PageInfo> = &mut *ALLOCATED_PAGES.lock().unwrap();

    // should i look for these concurrently? probably. will i? no.

    // look for reusable allocations
    if let Some(page) = pages.iter_mut().find(|page| has_reusable_allocation(page, size)) {
        return reuse_alloc(page, size);
    }

    // look for bump allocator space
    if let Some(page) = pages.iter_mut().find(|page| has_bump_space(page, size)) {
        return bump_alloc(page, size);
    }

    // if we got here, we need a new page
    let new_page: MmapMut = allocate_page();
    let new_page_addr: usize = new_page.as_ptr() as usize;
    let mut new_page_info: PageInfo = PageInfo {
        _page: new_page,
        page_addr: new_page_addr,
        page_size: PAGE_SIZE,
        bump_addr: new_page_addr,
        available_bump_size: PAGE_SIZE,
        n_allocations: 0,
        alloc_sizes: HashMap::new(),
        free_list: VecDeque::new(),
    };
    let alloc_addr = bump_alloc(&mut new_page_info, size);
    pages.push(new_page_info);
    return alloc_addr;
}

// free() frees the malloc()-allocated memory located at the provided pointer
#[no_mangle]
pub extern "C" fn gfree(addr: usize) {
    // lock the mutex
    let pages: &mut Vec<PageInfo> = &mut *ALLOCATED_PAGES.lock().unwrap();

    // find the page the allocation was within
    if let Some(page) = pages.iter_mut().find(|page| addr >= page.page_addr && addr < page.page_addr + page.page_size) {
        // find the allocation's size and remove it from the hashmap
        if let Some(size) = page.alloc_sizes.remove(&addr) {
            // add a new entry to the free list
            page.free_list.push_back(Mallocation {addr, size});
        }
    }
}