use std::ffi::{CStr, CString};
use std::os::raw::c_char;
use std::time::Instant;
use log::{info, LevelFilter};
use android_logger::Config;

#[no_mangle]
pub extern "C" fn rust_engine(input: *const c_char) -> *mut c_char {
    let start_time = Instant::now();

    android_logger::init_once(
        Config::default().with_max_level(LevelFilter::Trace).with_tag("CAKRU_RUST")
    );

    if input.is_null() {
        return CString::new("[LOG: Error] Input NULL").unwrap().into_raw();
    }

    let c_str = unsafe { CStr::from_ptr(input) };
    let input_str = c_str.to_str().unwrap_or("error");
    
    let mut log_ui = format!("[LOG: Perintah: '{}']\n", input_str);

    let response = match input_str.to_lowercase().as_str() {
        "laporan" => {
            log_ui.push_str("[LOG: Mengompilasi data laporan bulanan...]\n");
            let mut tabel = String::from("\n=== LAPORAN ODFIZ 2026 ===\n");
            tabel.push_str("| Item       | Qty | Status |\n");
            tabel.push_str("|------------|-----|--------|\n");
            tabel.push_str("| Gadget A   | 25  | Aman   |\n");
            tabel.push_str("| Sparepart  | 10  | Tipis  |\n");
            tabel.push_str("| Jasa Servis| 15  | Selesai|\n");
            tabel.push_str("==========================\n");
            tabel.push_str("Kesimpulan: Profit naik 12%\n");
            tabel
        },
        "tim" => {
            log_ui.push_str("[LOG: Membaca database tim maintenance...]\n");
            "Daftar Tim: Ajar, Dendi, Angga, Heru, Eko.\nStatus: Semua aktif.".to_string()
        },
        _ => {
            log_ui.push_str("[LOG: Memproses teks umum...]\n");
            format!("Rust menerima: '{}'.", input_str)
        },
    };

    let duration = start_time.elapsed();
    log_ui.push_str(&format!("[LOG: Selesai dalam {:?}]\n---\n", duration));
    
    let final_output = format!("{}{}", log_ui, response);
    
    info!("{}", final_output);
    CString::new(final_output).unwrap().into_raw()
}

#[no_mangle]
pub extern "C" fn rust_free_string(s: *mut c_char) {
    unsafe {
        if s.is_null() { return; }
        let _ = CString::from_raw(s);
    }
}
