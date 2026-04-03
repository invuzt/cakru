use std::ffi::{CStr, CString};
use std::os::raw::c_char;

#[no_mangle]
pub extern "C" fn rust_engine(input: *const c_char) -> *mut c_char {
    let c_str = unsafe { CStr::from_ptr(input) };
    let input_str = c_str.to_str().unwrap_or("error");

    // FOKUS DI SINI: Tambah fitur cukup di dalam match ini
    let response = match input_str.to_lowercase().as_str() {
        "cek stok" => "Rust: Stok Odfiz saat ini ada 150 unit.".to_string(),
        "halo" => "Rust: Halo Guru! Siap memproses data.".to_string(),
        "bersih" => "Rust: Perintah pembersihan diterima!".to_string(),
        _ => format!("Rust menerima input: '{}'. (Gunakan 'halo' atau 'cek stok')", input_str),
    };

    CString::new(response).unwrap().into_raw()
}
