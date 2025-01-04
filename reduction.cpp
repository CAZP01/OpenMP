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
    int total_sum = 0;

    // Menggunakan OpenMP untuk menghitung jumlah total dengan reduction
    // Sehingga race condition selama proses penambahan tidak terjadi karena adanya pemanfaatan penyimpanan lokal
    #pragma omp parallel for reduction(+:total_sum)
    for (int i = 0; i < SIZE; ++i) {
        int thread_id = omp_get_thread_num();
        
        // Menampilkan hasil selama proses reduction menggunakan critical untuk menghindari adanya race
        // Sebenarnya tanpa adanya critical, proses reduction akan tetap berjalan tanpa masalah
        // Namun karena kode yang berfungsi untuk menampilkan hasil selama proses reduction ada dalam looping
        // Maka ada kemungkinan bahwa hasilnya di terminal akan tidak beraturan
        // Sehingga penggunaan critical dapat mengurangi kemungkinan race ketika ingin menampilkan output
        #pragma omp critical
        {
            // Perintah utama untuk menampilkan hasil selama proses reduction
            std::cout << "Thread " << thread_id << " menambahkan array[" << i << "] = " << array[i] << "\n";
        }
        
        // Menyimpan hasil penambahan dalam variabel total_sum
        total_sum += array[i];
    }

    // Menampilkan total hasil penjumlahan secara biasa
    std::cout << "Jumlah total dari elemen array: " << total_sum << std::endl;

    return 0;
}
