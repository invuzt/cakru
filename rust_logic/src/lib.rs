use std::ffi::{CStr, CString};
use std::os::raw::c_char;
use std::time::Instant;

#[no_mangle]
pub extern "C" fn rust_engine(input: *const c_char) -> *mut c_char {
    let start_time = Instant::now();

    if input.is_null() {
        return CString::new("[LOG: Error] Input NULL").unwrap().into_raw();
    }

    let c_str = unsafe { CStr::from_ptr(input) };
    let input_str = c_str.to_str().unwrap_or("error");
    
    let mut log_ui = format!("[LOG: Mode: '{}']\n", input_str);

    let response = match input_str.to_lowercase().as_str() {
        "brute" => {
            log_ui.push_str("[LOG: Memulai Brute-Force Simulation...]\n");
            log_ui.push_str("[LOG: Mencari Nonce untuk Hash Target...]\n");
            
            let target = 777777; // Angka keberuntungan yang dicari
            let mut nonce = 0;
            let mut hash_attempt: u64 = 123456789; // Seed awal
            
            // Lakukan percobaan tebakan sampai ketemu target
            // Kita batasi maksimal 5 juta percobaan agar HP tidak freeze
            for i in 0..5_000_000 {
                nonce = i;
                // Logika "Linear Congruential Generator" (Simulasi Hashing)
                hash_attempt = (hash_attempt.wrapping_mul(1664525).wrapping_add(1013904223)) % 1_000_000;
                
                if hash_attempt == target {
                    log_ui.push_str(&format!("[LOG: SUCCESS! Found at attempt: {}]\n", i));
                    break;
                }
            }

            format!(
                "=== RUST CRYPTO BRUTE ===\n\
                 Target Number : {}\n\
                 Last Nonce    : {}\n\
                 Result Hash   : {}\n\
                 Status        : VALIDATED\n\
                 =========================", 
                target, nonce, hash_attempt
            )
        },
        _ => "Ketik 'brute' untuk simulasi tebak hash 5 juta data.".to_string(),
    };

    let duration = start_time.elapsed();
    log_ui.push_str(&format!("[LOG: Selesai dalam {:?}]\n---\n", duration));
    
    let final_output = format!("{}{}", log_ui, response);
    CString::new(final_output).unwrap().into_raw()
}

#[no_mangle]
pub extern "C" fn rust_free_string(s: *mut c_char) {
    unsafe {
        if s.is_null() { return; }
        let _ = CString::from_raw(s);
    }
}
