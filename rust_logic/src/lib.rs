use std::ffi::{CStr, CString};
use std::os::raw::c_char;

#[no_mangle]
pub extern "C" fn proses_teks_rust(input_ptr: *const c_char) -> *mut c_char {
    if input_ptr.is_null() { return std::ptr::null_mut(); }

    // 1. Ubah pointer C ke String Rust
    let c_str = unsafe { CStr::from_ptr(input_ptr) };
    let input_str = c_str.to_str().unwrap_or("Error");

    // 2. Logika: Tambah prefix (Contoh: Odfiz POS System)
    let hasil = format!("Odfiz Logic: Berhasil memproses [{}]", input_str);

    // 3. Ubah kembali ke C-style string untuk dikembalikan
    let c_result = CString::new(hasil).unwrap();
    c_result.into_raw()
}

// Tambahkan fungsi untuk membebaskan memori agar tidak bocor (Memory Leak)
#[no_mangle]
pub extern "C" fn free_rust_string(s: *mut c_char) {
    if s.is_null() { return; }
    unsafe {
        let _ = CString::from_raw(s);
    }
}
