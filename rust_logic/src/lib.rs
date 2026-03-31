use std::sync::atomic::{AtomicI32, Ordering};

static CPU_RAW: AtomicI32 = AtomicI32::new(50);
static RAM_RAW: AtomicI32 = AtomicI32::new(50);
static BAT_RAW: AtomicI32 = AtomicI32::new(50);
static TMP_RAW: AtomicI32 = AtomicI32::new(50);

#[no_mangle] pub extern "C" fn set_sys_data(c: i32, r: i32, b: i32, t: i32) {
    CPU_RAW.store(c, Ordering::SeqCst);
    RAM_RAW.store(r, Ordering::SeqCst);
    BAT_RAW.store(b, Ordering::SeqCst);
    TMP_RAW.store(t, Ordering::SeqCst);
}

#[no_mangle] pub extern "C" fn get_cpu_usage() -> f32 { CPU_RAW.load(Ordering::SeqCst) as f32 / 100.0 }
#[no_mangle] pub extern "C" fn get_ram_usage() -> f32 { RAM_RAW.load(Ordering::SeqCst) as f32 / 100.0 }
#[no_mangle] pub extern "C" fn get_bat_level() -> f32 { BAT_RAW.load(Ordering::SeqCst) as f32 / 100.0 }
#[no_mangle] pub extern "C" fn get_temp_level() -> f32 { TMP_RAW.load(Ordering::SeqCst) as f32 / 100.0 }
#[no_mangle] pub extern "C" fn load_stok() {}
