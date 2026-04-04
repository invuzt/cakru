use std::ffi::{CStr, CString};
use std::os::raw::c_char;

#[no_mangle]
pub extern "C" fn rust_engine(input: *const c_char) -> *mut c_char {
    let c_str = unsafe { CStr::from_ptr(input) };
    let input_str = c_str.to_str().unwrap_or("error");

    // Logika Utama: Tambahkan fitur di sini
    let response = match input_str.to_lowercase().as_str() {
        "cek stok" => "Rust: Stok Odfiz saat ini ada 150 unit.".to_string(),
        "halo" => "Rust: Halo Guru! Siap memproses data.".to_string(),
        "bersih" => "Rust: Perintah pembersihan diterima!".to_string(),
        "jadwal" => "Rust: Jadwal besok adalah Maintenance shift pagi.".to_string(),
        _ => format!("Rust menerima input: '{}'. (Gunakan 'halo', 'cek stok', atau 'jadwal')", input_str),
    };

    // Mengubah String menjadi pointer C yang bisa dibaca JNI
    CString::new(response).unwrap().into_raw()
}

#[no_mangle]
pub extern "C" fn rust_free_string(s: *mut c_char) {
    unsafe {
        if s.is_null() { return; }
        // Mengambil kembali kepemilikan string agar Rust bisa menghapusnya dari RAM
        let _ = CString::from_raw(s);
    }
}
