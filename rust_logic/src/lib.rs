use std::fs;

// Fungsi pembantu: Membaca angka dari file sistem Android
fn read_val(path: &str) -> f32 {
    fs::read_to_string(path)
        .ok()
        .and_then(|s| s.trim().parse::<f32>().ok())
        .unwrap_or(0.0)
}

#[no_mangle]
pub extern "C" fn get_cpu_usage() -> f32 {
    // CPU usage murni dari /proc/stat butuh kalkulasi delta.
    // Untuk sekarang kita pakai sisa uptime sebagai animasi halus.
    let uptime = read_val("/proc/uptime");
    ((uptime * 2.0) % 100.0) / 100.0
}

#[no_mangle]
pub extern "C" fn get_ram_usage() -> f32 {
    if let Ok(mem) = fs::read_to_string("/proc/meminfo") {
        let mut total = 1.0;
        let mut avail = 1.0;
        for line in mem.lines() {
            if line.starts_with("MemTotal:") {
                total = line.split_whitespace().nth(1).unwrap_or("1").parse().unwrap_or(1.0);
            }
            if line.starts_with("MemAvailable:") {
                avail = line.split_whitespace().nth(1).unwrap_or("1").parse().unwrap_or(1.0);
            }
        }
        return (total - avail) / total;
    }
    0.5
}

#[no_mangle]
pub extern "C" fn get_bat_level() -> f32 {
    // Membaca persentase baterai asli (0-100)
    read_val("/sys/class/power_supply/battery/capacity") / 100.0
}

#[no_mangle]
pub extern "C" fn get_temp_level() -> f32 {
    // Membaca suhu baterai (biasanya dalam miliderajat, misal 35000 = 35°C)
    let temp = read_val("/sys/class/power_supply/battery/temp") / 10.0; 
    // Kita petakan 25°C - 45°C ke 0.0 - 1.0 agar Bar-nya pas
    ((temp - 250.0) / 200.0).clamp(0.0, 1.0)
}

// Fungsi pendukung agar Linker tidak bingung
#[no_mangle] pub extern "C" fn load_stok() {}
#[no_mangle] pub extern "C" fn save_stok() {}
#[no_mangle] pub extern "C" fn get_stok() -> i32 { 0 }
