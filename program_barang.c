#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define MAX 100

struct Barang {
    char nama[50];
    int jumlah;
    float harga;
};

struct Riwayat {
    char keterangan[100];
    char waktu[30];
};

// ====== FUNGSI TAMBAHAN UNTUK FILE ======

// Simpan data barang ke file
void simpanData(struct Barang data[], int n) {
    FILE *f = fopen("data.txt", "w");
    if (f == NULL) {
        printf("Gagal membuka file data.txt untuk menulis!\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        fprintf(f, "%s|%d|%.2f\n", data[i].nama, data[i].jumlah, data[i].harga);
    }
    fclose(f);
}

// Muat data barang dari file
int muatData(struct Barang data[]) {
    FILE *f = fopen("data.txt", "r");
    if (f == NULL) {
        return 0; // jika belum ada file
    }
    int n = 0;
    while (fscanf(f, "%49[^|]|%d|%f\n", data[n].nama, &data[n].jumlah, &data[n].harga) == 3) {
        n++;
    }
    fclose(f);
    return n;
}

// Simpan riwayat ke file
void simpanRiwayat(struct Riwayat r[], int count) {
    FILE *f = fopen("riwayat.txt", "w");
    if (f == NULL) {
        printf("Gagal membuka file riwayat.txt untuk menulis!\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(f, "%s|%s\n", r[i].waktu, r[i].keterangan);
    }
    fclose(f);
}

// Muat riwayat dari file
int muatRiwayat(struct Riwayat r[]) {
    FILE *f = fopen("riwayat.txt", "r");
    if (f == NULL) return 0;
    int count = 0;
    while (fscanf(f, "%29[^|]|%99[^\n]\n", r[count].waktu, r[count].keterangan) == 2) {
        count++;
    }
    fclose(f);
    return count;
}

// ====== FUNGSI UTAMA PROGRAM ======

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waktuSekarang(char *buffer) {
    time_t now;
    struct tm *t;
    time(&now);
    t = localtime(&now);
    strftime(buffer, 30, "%d-%m-%Y %H:%M:%S", t);
}

int stricmp_custom(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return tolower((unsigned char)*a) - tolower((unsigned char)*b);
        a++;
        b++;
    }
    return *a - *b;
}

void tampilkanBarang(struct Barang data[], int n) {
    printf("================= DAFTAR BARANG =================\n");
    if (n == 0) {
        printf("Belum ada data barang.\n");
    } else {
        printf("%-3s %-20s %-10s %-10s\n", "No", "Nama Barang", "Jumlah", "Harga");
        for (int i = 0; i < n; i++) {
            printf("%-3d %-20s %-10d Rp.%-10.2f\n", i + 1, data[i].nama, data[i].jumlah, data[i].harga);
        }
    }
    printf("--------------------------------------------------\n");
}

void tampilkanRiwayat(struct Riwayat r[], int count) {
    printf("=============== RIWAYAT TRANSAKSI ===============\n");
    if (count == 0) {
        printf("Belum ada transaksi.\n");
    } else {
        for (int i = 0; i < count; i++) {
            printf("[%s] %s\n", r[i].waktu, r[i].keterangan);
        }
    }
}    

// Inisialisasi data awal (jika file belum ada)
void inisialisasiData(struct Barang data[], int *n) {
    struct Barang awal[10] = {
        {"Oli Mesin", 25, 75000},
        {"Busi", 25, 20000},
        {"Filter Udara", 25, 35000},
        {"Kampas Rem", 25, 45000},
        {"Ban Luar", 25, 300000},
        {"Ban Dalam", 25, 100000},
        {"Aki Motor", 25, 250000},
        {"Lampu Depan", 25, 50000},
        {"Rantai", 25, 85000},
        {"Helm", 25, 250000}
    };

    for (int i = 0; i < 10; i++) {
        data[i] = awal[i];
    }
    *n = 10;
}

int main() {
    struct Barang data[MAX];
    struct Riwayat riwayat[200];
    int n, jumlahRiwayat;
    int pilihan;
    char namaCari[50];
    int i, found;

    // === Muat data dari file jika ada ===
    n = muatData(data);
    jumlahRiwayat = muatRiwayat(riwayat);

    // Jika file kosong, isi dengan data awal
    if (n == 0) {
        inisialisasiData(data, &n);
        simpanData(data, n);
    }

    while (1) {
        clearScreen();
        tampilkanBarang(data, n);

        printf("\n===== MENU PENYIMPANAN BARANG =====\n");
        printf("|1.             Tambah Barang Baru|\n");
        printf("|2.             Tambah Stok Barang|\n");
        printf("|3.                 Kurangi Barang|\n");
        printf("|4.                   Hapus Barang|\n");
        printf("|5.        Lihat Riwayat Transaksi|\n");
        printf("|6.                         Keluar|\n");
        printf("===================================\n");
        printf("Pilih menu (1-6): ");
        scanf("%d", &pilihan);
        getchar();

        clearScreen();

        switch (pilihan) {
            case 1: {
                tampilkanBarang(data, n);
                printf("\n=== TAMBAH BARANG BARU ===\n");
                if (n < MAX) {
                    printf("Masukkan nama barang: ");
                    fgets(data[n].nama, sizeof(data[n].nama), stdin);
                    data[n].nama[strcspn(data[n].nama, "\n")] = 0;

                    printf("Masukkan jumlah barang: ");
                    scanf("%d", &data[n].jumlah);

                    printf("Masukkan harga barang: ");
                    scanf("%f", &data[n].harga);
                    getchar();

                    char waktu[30];
                    waktuSekarang(waktu);
                    sprintf(riwayat[jumlahRiwayat].keterangan,
                            "Menambahkan barang baru '%s' sebanyak %d (Rp%.2f)",
                            data[n].nama, data[n].jumlah, data[n].harga);
                    strcpy(riwayat[jumlahRiwayat].waktu, waktu);
                    jumlahRiwayat++;

                    n++;
                } else {
                    printf("Data penuh!\n");
                }
                break;
            }

            case 2: {
                tampilkanBarang(data, n);
                printf("\n=== TAMBAH STOK BARANG ===\n");
                printf("Masukkan nama barang: ");
                fgets(namaCari, sizeof(namaCari), stdin);
                namaCari[strcspn(namaCari, "\n")] = 0;

                found = 0;
                for (i = 0; i < n; i++) {
                    if (stricmp_custom(data[i].nama, namaCari) == 0) {
                        int tambah;
                        printf("Masukkan jumlah tambahan: ");
                        scanf("%d", &tambah);
                        getchar();
                        data[i].jumlah += tambah;

                        char waktu[30];
                        waktuSekarang(waktu);
                        sprintf(riwayat[jumlahRiwayat].keterangan,
                                "Menambah stok barang '%s' sebanyak %d",
                                data[i].nama, tambah);
                        strcpy(riwayat[jumlahRiwayat].waktu, waktu);
                        jumlahRiwayat++;

                        found = 1;
                        break;
                    }
                }
                if (!found)
                    printf("Barang tidak ditemukan.\n");
                break;
            }

            case 3: {
                tampilkanBarang(data, n);
                printf("\n=== KURANGI BARANG ===\n");
                printf("Masukkan Nama Barang: ");
                fgets(namaCari, sizeof(namaCari), stdin);
                namaCari[strcspn(namaCari, "\n")] = 0;

                found = 0;
                for (i = 0; i < n; i++) {
                    if (stricmp_custom(data[i].nama, namaCari) == 0) {
                        int kurang;
                        printf("Masukkan Jumlah yang Dikurangi: ");
                        scanf("%d", &kurang);
                        getchar();
                        if (kurang <= data[i].jumlah) {
                            data[i].jumlah -= kurang;

                            char waktu[30];
                            waktuSekarang(waktu);
                            sprintf(riwayat[jumlahRiwayat].keterangan,
                                    "Mengurangi barang '%s' sebanyak %d",
                                    data[i].nama, kurang);
                            strcpy(riwayat[jumlahRiwayat].waktu, waktu);
                            jumlahRiwayat++;
                        } else {
                            printf("Jumlah Pengurangan Melebihi Stok!\n");
                        }
                        found = 1;
                        break;
                    }
                }
                if (!found)
                    printf("BARANG TIDAK DITEMUKAN!!!\n");
                break;
            }

            case 4: {
                tampilkanBarang(data, n);
                printf("\n=== HAPUS BARANG ===\n");
                printf("Masukkan Nama Barang: ");
                fgets(namaCari, sizeof(namaCari), stdin);
                namaCari[strcspn(namaCari, "\n")] = 0;

                found = 0;
                for (i = 0; i < n; i++) {
                    if (stricmp_custom(data[i].nama, namaCari) == 0) {
                        char waktu[30];
                        waktuSekarang(waktu);
                        sprintf(riwayat[jumlahRiwayat].keterangan,
                                "Menghapus Barang '%s'", data[i].nama);
                        strcpy(riwayat[jumlahRiwayat].waktu, waktu);
                        jumlahRiwayat++;

                        for (int j = i; j < n - 1; j++) {
                            data[j] = data[j + 1];
                        }
                        n--;
                        found = 1;
                        break;
                    }
                }
                if (!found)
                    printf("BARANG TIDAK DITEMUKAN!!!\n");
                break;
            }

            case 5:
                tampilkanRiwayat(riwayat, jumlahRiwayat);
                printf("\nTekan Enter untuk Kembali !!!");
                getchar();
                break;

            case 6:
                simpanData(data, n);
                simpanRiwayat(riwayat, jumlahRiwayat);
                clearScreen();
                printf("Data dan riwayat telah disimpan ke file.\n");
                printf("TERIMA KASIH !!!! Program Selesai.\n");
                return 0;

            default:
                printf("Pilihan tidak valid!\n");
        }

        // Simpan setiap perubahan otomatis
        simpanData(data, n);
        simpanRiwayat(riwayat, jumlahRiwayat);
    }
}
