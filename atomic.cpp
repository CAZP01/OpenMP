#include <iostream>
#include <omp.h>

int main() {
    // Variabel untuk menentukan jumlah array
    const int SIZE = 20;
    int array[SIZE];

    // Mengisi array dengan nilai dari 1 hingga SIZE
    for (int i = 0; i < SIZE; ++i) {
        array[i] = i + 1;
    }

    // Variabel untuk menyimpan hasil penjumlahan
    int sum = 0;

    // Menjalankan operasi penambahan secara paralel menggunakan atomic
    #pragma omp parallel
    {
        // Mengatur agar semua thread bisa digunakan
        #pragma omp for
        for (int i = 0; i < SIZE; ++i) {
            int thread_id = omp_get_thread_num();  // Menyimpan ID thread yang melakukan eksekusi aritmatik
            
            // Penambahan dengan menerapkan atomic
            // Sehingga hanya satu thread yang bisa akses proses penambahan
            #pragma omp atomic
            sum += array[i];

            // Menampilkan hasil selama proses atomic menggunakan critical untuk menghindari adanya race
            // Sebenarnya tanpa adanya critical, proses atomic akan tetap berjalan tanpa masalah
            // Namun karena kode yang berfungsi untuk menampilkan hasil selama proses atomic ada dalam looping
            // Maka ada kemungkinan bahwa hasilnya di terminal akan tidak beraturan
            // Sehingga penggunaan critical dapat mengurangi kemungkinan race ketika ingin menampilkan output
            #pragma omp critical
            {
                // Perintah utama untuk menampilkan hasil selama proses atomic
                std::cout << "Thread " << thread_id << " menambahkan array[" << i << "] = " << array[i] << std::endl;
            }
        }
    }

    // Menampilkan total hasil penjumlahan secara biasa
    std::cout << "Jumlah total (penambahan) : " << sum << std::endl;

    return 0;
}
