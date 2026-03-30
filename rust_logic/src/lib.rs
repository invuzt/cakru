use std::os::raw::{c_int, c_uchar};

#[no_mangle]
pub extern "C" fn cakru_process_image(
    buffer: *mut c_uchar, 
    width: c_int, 
    height: c_int
) {
    let w = width as usize;
    let h = height as usize;
    let size = (w * h * 4) as usize;

    unsafe {
        let pixels = std::slice::from_raw_parts_mut(buffer, size);

        // Area Watermark: 25% dari lebar, 10% dari tinggi
        let wm_w = w / 4;
        let wm_h = h / 10;
        let start_x = w - wm_w - 50;
        let start_y = h - wm_h - 50;

        for y in start_y..(start_y + wm_h) {
            for x in start_x..(start_x + wm_w) {
                let idx = (y * w + x) * 4;
                if idx + 3 < size {
                    // Logika Pixel Blending: 
                    // Kita buat efek semi-transparan dengan menggeser bit (Fast Math)
                    pixels[idx]     = (pixels[idx] >> 1) + 120;     // Red
                    pixels[idx + 1] = (pixels[idx + 1] >> 1) + 120; // Green
                    pixels[idx + 2] = (pixels[idx + 2] >> 1) + 120; // Blue
                    // pixels[idx + 3] adalah Alpha, biarkan tetap 255
                }
            }
        }
    }
}
