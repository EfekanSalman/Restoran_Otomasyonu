#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>

#define RESTORAN_SAYISI 5 

//Kullanici sinifi
struct User {
	char name[50];
	char surname[50];
	char username[50];
	char password[50];
	
	int age;
	char gender; 
	
	char city[50];
	char ilce[50];
	char sokak[50];
	char apartmanIsmi[50];
	int apartmanNo;
	int daireNo;
	
	char cardOwner[50];
	char cardNumber[20];
	char validityDate[6];
	char securityCode[6];
};

//Restoran sinifi
struct Restaurant {
	int id;
	char name[20];
	int masalar[10];
	float puanlar[10]; 
	char yorum[500]; 
	char isim[100];
};

//Restoranlarin sinifi
struct Restaurant restaurants[RESTORAN_SAYISI] = {
	{1, "Küçükçekemece", {0}},
	{2, "Besiktas", {0}},
	{3, "Sultangazi", {0}},
	{4, "Basaksehir", {0}},
	{5, "Tuzla", {0}}	
};

//Rezervasyon sinifi
struct Reservation {
	int restaurant_id;
	char purpose[20];
	int num_people;
	int day_choice;
	char reservation_time[10];
	int masa;
};

//Ürün sinifi
struct Urun {
	char ad[50];
	float fiyat;
	int adet;
	int stok;
};

//Sepet sinifi
struct Sepet {
	struct Urun urunler[50];
	int urun_sayisi;
	float toplam_tutar;
};

//Menü sinifi
struct MenuItem {
	char ad[50];
	float fiyat;
};


//menü ögelerin sinifi
struct MenuItem menu[8][8] = {
    {{"Kahvalti Tabagi", 300}, {"Serpme Kahvalti", 500}, {"Sahanda Yumurta", 100}, {"Sucuklu Yumurta", 150}, {"Pankek Tabagi", 200}, {"Pisi Tabagi", 300}, {"Menemen", 300}, {"Omlet", 120}},
    {{"Adana Kebap", 310}, {"Urfa Kebap", 310}, {"Kuzu Sis", 330}, {"Çöp Sis", 330}, {"Patlican Kebabi", 400}, {"Tavuk Sis", 145}, {"Tavuk Kanat", 250}, {"Sac tava", 310}},
    {{"Mercimek Çorbasi", 75}, {"Domates Çorbasi", 75}, {"Tavuk Suyu Çorbasi", 75}, {"Kelle Paça Çorbasi", 165}, {"Beyran", 165}, {"Mantar Çorbasi", 65}, {"Yayla Çorbasi", 75}, {"Yüksük Çorbasi", 80}},
    {{"Acili Ezme", 40}, {"Cacik", 150}, {"Girit Ezmesi", 400}, {"Humus", 40}, {"Haydari", 40}, {"Zeytin Piyazi", 60}, {"Patlican Salatasi", 50}, {"Çig köfte", 70}},
    {{"Mevsim Salata", 60}, {"Nurdagi Salata", 80}, {"Çoban Salatasi", 50}, {"Yesil Salata", 45}, {"Gavurdagi Salatasi", 100}, {"Kasik Salata", 70}, {"Sezar Salata", 70}, {"Rus Salatasi", 120}},
    {{"Kuru Patlican Dolmasi", 75}, {"Kizartma Içli Köfte", 90}, {"Haslama Içli Köfte", 95}, {"Patates Köftesi", 65}, {"Mücver", 170}, {"Sigara böregi", 35}, {"Paçanga böregi", 50}, {"Findik lahmacun", 70}},
    {{"Sicak Burma Kadayif", 300}, {"Gaziantep Katmeri", 280}, {"Fistikli Baklava", 270}, {"Fistikli Sarma", 300}, {"Midye Baklava", 260}, {"Cevizli Baklava", 160}, {"Dondurmali Havuç Dilimi", 240}, {"Söbiyet", 200}},
    {{"Coca Cola", 80}, {"Ice Tea", 55}, {"Sprite", 70}, {"Soda", 35}, {"Limonata", 75}, {"Ayran", 50}, {"Salgam", 40}, {"Sira", 80}}
};

//====================================================================================================================================================================


//Promoson sinifi
struct Promosyon {
	char kod[20];
	float indirim_orani;
};


//Kod promosyonlarin sinifi
struct Promosyon promosyonlar[] = {
	{"YENI20", 0.20}, //%20 indirim
	{"OGRENCI", 0.40}, //%40 indirim
	{"BILALINAN", 0.95} //%95 indirim -- az bile
};


//Siniflar bitti 


//Günleri tanimladik
char * days[] = {
	"Pazartesi",
	"Sali",
	"Çarsamba",
	"Persembe",
	"Cuma",
	"Cumartesi",
	"Pazar"
};


// Stoklari baslatma
int stoklar[8][8] = {
    {30, 55, 20, 25, 35, 35, 40, 45},
    {30, 55, 20, 25, 35, 35, 40, 45},
    {30, 55, 20, 25, 35, 35, 40, 45},
    {30, 55, 20, 25, 35, 35, 40, 45},
    {30, 55, 20, 25, 35, 35, 40, 45},
    {30, 55, 20, 25, 35, 35, 40, 45},
    {30, 55, 20, 25, 35, 35, 40, 45},
    {30, 55, 20, 25, 35, 35, 40, 45}
};


//voidler buraya gelsin ===========================================================================

//kullanicinin bilgilerini kaydeder
void saveUser(const struct User *user) {
    FILE *file = fopen("user_info.txt", "a");
    if (file == NULL) {
        printf("Dosya açilamadi");
        return;
    }

    fprintf(file, "%s %s %s %s %s %s %s %s %d %d %s %s %s %s\n", user->name, user->surname, user->username, user->password,
            user->city, user->ilce, user->sokak, user->apartmanIsmi, user->apartmanNo, user->daireNo,
            user->cardOwner, user->cardNumber, user->validityDate, user->securityCode);
    fclose(file);
}

//Kayit olmus bilgileri ekrana yazdirir
void printUsers() {
    FILE *file = fopen("user_info.txt", "r");
    if (file == NULL) {
        printf("Dosya açilamadi");
        return;
    }

    struct User user;
    while (fscanf(file, "%s %s %s %s %s %s %s %s %d %d %s %s %s %s\n", user.name, user.surname, user.username,
                  user.password, user.city, user.ilce, user.sokak, user.apartmanIsmi, &user.apartmanNo, &user.daireNo,
                  user.cardOwner, user.cardNumber, user.validityDate, user.securityCode) != EOF) {
        printf("Isminiz: %s\n", user.name);
        printf("Soyadiniz: %s\n", user.surname);
        printf("Kullanici Adiniz: %s\n", user.username);
        printf("Sifreniz: %s\n", user.password);
        printf("Sehir: %s\n", user.city);
        printf("Ilçe: %s\n", user.ilce);
        printf("Sokak: %s\n", user.sokak);
        printf("Apartman Ismi: %s\n", user.apartmanIsmi);
        printf("Apartman No: %d\n", user.apartmanNo);
        printf("Daire No: %d\n", user.daireNo);
        printf("Kart Sahibi: %s\n", user.cardOwner);
        printf("Kart Numarasi: %s\n", user.cardNumber);
        printf("Geçerlilik Tarihi: %s\n", user.validityDate);
        printf("Güvenlik Kodu: %s\n", user.securityCode);
    }
    fclose(file);
}

//Kullanici sayfasi
void printUserInfo(const struct User *user) {
	printf("            44444444                        Adiniz: %s\n", user->name);
    printf("          444444444444                      Soyadiniz: %s\n", user->surname);
    printf("         44444444444444                     Kullanici adiniz: %s\n", user->username);
    printf("         44444444444444                     Sifreniz: %s\n", user->password);
    printf("         44444444444444                     Oturdugunuz sehir: %s\n", user->city);
    printf("         44444444444444                     Oturdugunuz ilce: %s\n", user->ilce);
    printf("          444444444444                      Oturdugunuz sokak: %s\n", user->sokak);
    printf("          4444444444                        Oturdugunuz apartmanin ismi: %s\n", user->apartmanIsmi);
    printf("            44444444                        Oturdugunuz apartman no: %d\n", user->apartmanNo);
    printf("            44444444                        Daire no: %d\n", user->daireNo);
    printf("         44444444444444                     Kartin üstündeki isim: %s\n", user->cardOwner);
    printf("      44444444444444444444                  Kartin numarasi: %s\n", user->cardNumber);
    printf("    444444444444444444444444                Kartin Gecerlilik tarihi: %s\n", user->validityDate);
    printf("   44444444444444444444444444               Kartin güvenlik kodu: %s\n", user->securityCode);
}


//Giris yapma fonksiyonu      sorununun ana nedeni burada ve bitti gibi gözüküyor yani umarim
void loginUser(struct User *user) {
    system("cls"); 

    char username[50];
    char password[50];
	
    // Kullanicidan kullanici adi ve sifreyi alalim
    printf("\t\t\t\t\t\t --------------------------\n");
    printf("\t\t\t\t\t\t Kullanici Adi: ");
    scanf("%s", username);
    printf("\t\t\t\t\t\t --------------------------\n");
    printf("\t\t\t\t\t\t Sifre: ");
    scanf("%s", password);
    
    // Kullanici bilgilerini tutacak bir yapi olusturalim
    struct User currentUser;

    // Kullanici bilgilerini içeren dosyayi açalim
    FILE *file = fopen("user_info.txt", "r");
    if (file == NULL) {
        printf("Hesap bulunamadi.\n");
        return;
    }

    // Dosyadan sirayla kullanici bilgilerini okuyalim
    while (fscanf(file, "%s %s %s %s %s %s %s %s %d %d %s %s %s %s\n", currentUser.name, currentUser.surname, currentUser.username,
                  currentUser.password, currentUser.city, currentUser.ilce, currentUser.sokak, currentUser.apartmanIsmi, &currentUser.apartmanNo, &currentUser.daireNo,
                  currentUser.cardOwner, currentUser.cardNumber, currentUser.validityDate, currentUser.securityCode) != EOF) {
        // Kullanici adi ve sifreyi kontrol edelim
        if (strcmp(username, currentUser.username) == 0 && strcmp(password, currentUser.password) == 0) {
            printf("\t\t\t\t\t\t Giris basarili!\n");
            // Kullanici bilgilerini user yapisina kopyalayalim
            strcpy(user->name, currentUser.name);
            strcpy(user->surname, currentUser.surname);
            strcpy(user->username, currentUser.username);
            strcpy(user->password, currentUser.password);
            strcpy(user->city, currentUser.city);
            strcpy(user->ilce, currentUser.ilce);
            strcpy(user->sokak, currentUser.sokak);
            strcpy(user->apartmanIsmi, currentUser.apartmanIsmi);
            user->apartmanNo = currentUser.apartmanNo;
            user->daireNo = currentUser.daireNo;
            strcpy(user->cardOwner, currentUser.cardOwner);
            strcpy(user->cardNumber, currentUser.cardNumber);
            strcpy(user->validityDate, currentUser.validityDate);
            strcpy(user->securityCode, currentUser.securityCode);
            fclose(file);
            return;
        }
    }

    // Eger buraya kadar gelindi ise, giris bilgileri yanlis demektir
    printf("Hesap bulunamadi. Lütfen önce kayit olun.\n");
    fclose(file);
}

bool checkCredentials(const struct User *user) {
    FILE *file = fopen("user_info.txt", "r");
    if (file == NULL) {
        printf("Dosya açilamadi");
        return false;
    }

    struct User tempUser;
    while (fscanf(file, "%*s %*s %s %s %*s %*s %*s %*s %*d %*d %*s %*s %*s %*s\n", tempUser.username, tempUser.password) != EOF) {
        if (strcmp(tempUser.username, user->username) == 0 && strcmp(tempUser.password, user->password) == 0) {
            fclose(file);
            return true;
        }
    }

    fclose(file);
    return false;
}
//bu kod çalismiyor cünkü bu koduda kaldirdim
bool isEmailValid(const char *email) {
    int i = 0;
    while (email[i] != '\0') {
        if (email[i] == '@') {
            return true;
        }
        i++;
    }
    return false;
}

bool userExists(const struct User *user) {
    FILE *file = fopen("user_info.txt", "r");
    if (file == NULL) {
        printf("Dosya açilamadi!");
        return false;
    }

    struct User tempUser;
    while (fscanf(file, "%s %s %s %s %d %c %s %s %s %s %d %d %s %s %s %s\n", tempUser.name, tempUser.surname, tempUser.username,
                  tempUser.password, &tempUser.age, &tempUser.gender, tempUser.city, tempUser.ilce, tempUser.sokak, tempUser.apartmanIsmi,
                  &tempUser.apartmanNo, &tempUser.daireNo, tempUser.cardOwner, tempUser.cardNumber, tempUser.validityDate, tempUser.securityCode) != EOF) {
        if (strcmp(tempUser.username, user->username) == 0 || (strcmp(tempUser.cardNumber, user->cardNumber) == 0 && strcmp(tempUser.cardOwner, user->cardOwner) == 0)) {
            fclose(file);
            return true;
        }
    }

    fclose(file);
    return false;
}

void registerUser() {
    //system("cls");
	struct User user;
    puts("\t\t\t\t\t\t--------------<0>--------------"); 
	printf("\t\t\t\t\t\tIsminiz: ");
    scanf("%s", user.name);
    puts("\t\t\t\t\t\t-------------------------------");
	printf("\t\t\t\t\t\tSoyadiniz: ");
    scanf("%s", user.surname);
    puts("\t\t\t\t\t\t-------------------------------");
	printf("\t\t\t\t\t\tKullanici Adiniz: ");
    scanf("%s", user.username);
    puts("\t\t\t\t\t\t-------------------------------");
	printf("\t\t\t\t\t\tSifreniz: ");
    scanf("%s", user.password);
    puts("\t\t\t\t\t\t-------------------------------");
	printf("\t\t\t\t\t\tSehir: ");
    scanf("%s", user.city);
    puts("\t\t\t\t\t\t-------------------------------");
	printf("\t\t\t\t\t\tIlçe: ");
    scanf("%s", user.ilce);
    puts("\t\t\t\t\t\t-------------------------------");
	printf("\t\t\t\t\t\tSokak: ");
    scanf("%s", user.sokak);
    puts("\t\t\t\t\t\t-------------------------------");
	printf("\t\t\t\t\t\tApartman Ismi: ");
    scanf("%s", user.apartmanIsmi);
    puts("\t\t\t\t\t\t-------------------------------");
	printf("\t\t\t\t\t\tApartman No: ");
    scanf("%d", &user.apartmanNo);
    puts("\t\t\t\t\t\t-------------------------------");
	printf("\t\t\t\t\t\tDaire No: ");
    scanf("%d", &user.daireNo);
    puts("\t\t\t\t\t\t-------------------------------");
	printf("\t\t\t\t\t\tKart Sahibi(Birlesik yaziniz): "); 
    scanf("%s", user.cardOwner);
    puts("\t\t\t\t\t\t-------------------------------");
	printf("\t\t\t\t\t\tKart Numarasi(16 Haneli): "); 
    scanf("%s", user.cardNumber);
    puts("\t\t\t\t\t\t-------------------------------");
	printf("\t\t\t\t\t\tGeçerlilik Tarihi AY/YIL: "); 
    scanf("%s", user.validityDate);
    puts("\t\t\t\t\t\t-------------------------------");
	printf("\t\t\t\t\t\tGüvenlik Kodu: ");
    scanf("%s", user.securityCode);
    puts("\t\t\t\t\t\t--------------<0>--------------");

    if (!checkCredentials(&user)) {
        if (userExists(&user)) {
            printf("\t\t\t\t\t\tBu kullanici zaten kayitli!\n");
        } 
		else {
            saveUser(&user);
            printf("\t\t\t\t\t\tKayit basarili.\n");
        }
    } 
	else {
        printf("\t\t\t\t\t\tBu kullanici zaten mevcut.\n");
    }
}


void updateUserInfo(struct User *user){
    int updateChoice;
	
	printf("\nBilgilerim dogru:\n");
    printf("1) Adres Bilgimi degistir \n");
    printf("2) Kart bilgilerimi degistir \n");
    printf("3) Sifremi degistir \n");
    printf("4) Geri Dön\n");
    scanf("%d", &updateChoice);
    
    switch(updateChoice){
        case 1:
            printf("\t\t\t\t\t\tOturdugunuz sehir: ");
            scanf("%s", user -> city);
            printf("\t\t\t\t\t\tOturdugunuz ilçe: ");
            scanf("%s", user -> ilce);
            printf("\t\t\t\t\t\tOturdugunuz sokak: ");
            scanf("%s", user ->sokak);
            printf("\t\t\t\t\t\tOturdugunuz apartmanin ismi: ");
            scanf("%s", user -> apartmanIsmi);
            printf("\t\t\t\t\t\tOturdugunuz apartman no: ");
            scanf("%d", &user -> apartmanNo);
            printf("\t\t\t\t\t\tDaire no: ");
            scanf("%d", &user -> daireNo);
            break;
        case 2:
            printf("\t\t\t\t\t\tKartin üzerindeki isim: ");
            scanf("%s", user -> cardOwner);
            printf("\t\t\t\t\t\tKartin üzerindeki sihirli rakamlar: ");
            scanf("%d", &user -> cardNumber );
            printf("\t\t\t\t\t\tKartin son kullanma tarihi: ");
            scanf("%d", &user -> validityDate);
            printf("\t\t\t\t\t\tKartin ccv giriniz: ");
            scanf("%d", &user -> securityCode);
            break;
        case 3:
            printf("\t\t\t\t\t\tHesabiniz yeni sifresini giriniz: ");
            scanf("%s", user -> password);
            break;
        case 4:
            break;
        default:
            printf("Geçersiz seçenek!\n");
    }
}


void printUserMenu() {
    //system("cls");
	printf("\n\t\t\t\t\t\t-- Kullanici Menu --\n");
	puts("\t\t\t\t\t\t-------------------------------");
    printf("\t\t\t\t\t\t1) Bilgilerimi Gor            |\n");
    puts("\t\t\t\t\t\t-------------------------------");
    printf("\t\t\t\t\t\t2) Siparis Ver                |\n");
    puts("\t\t\t\t\t\t-------------------------------");
    printf("\t\t\t\t\t\t3) Rezervasyon                |\n");
    puts("\t\t\t\t\t\t-------------------------------");
    printf("\t\t\t\t\t\t4) Sepetimi Gor               |\n");
    puts("\t\t\t\t\t\t-------------------------------");
    printf("\t\t\t\t\t\t5) Odeme Yap                  |\n");
    puts("\t\t\t\t\t\t-------------------------------");
    printf("\t\t\t\t\t\t6) Iptal Etme                 |\n");
    puts("\t\t\t\t\t\t-------------------------------");
    printf("\t\t\t\t\t\t7) Promosyon                  |\n");
    puts("\t\t\t\t\t\t-------------------------------");
    printf("\t\t\t\t\t\t8) Hakkimizda                 |\n");
    puts("\t\t\t\t\t\t-------------------------------");
    printf("\t\t\t\t\t\t9) Yorum Yap                  |\n"); 
	puts("\t\t\t\t\t\t-------------------------------");  
	printf("\t\t\t\t\t\t10)Populer Menüler            |\n");
	puts("\t\t\t\t\t\t-------------------------------");
	printf("\t\t\t\t\t\t11)Güvenli Çikis              |\n"); //veganda eklenicek
	puts("\t\t\t\t\t\t-------------------------------");
    printf("\t\t\t\t\t\tSeciminiz: ");
}


int kontrolEtPromosyon(char *kod) {
    for (int i = 0; i < sizeof(promosyonlar) / sizeof(promosyonlar[0]); i++) {
        if (strcmp(kod, promosyonlar[i].kod) == 0) {
            return i; // Eslesme bulundu, promosyonun indeksini döndür
        }
    }
    return -1; // Eslesme bulunamadi
}


void uygulaPromosyon(struct Sepet *sepet, char *kod) {
    int promosyonIndex = kontrolEtPromosyon(kod);
    if (promosyonIndex != -1) {
        float indirimMiktari = sepet->toplam_tutar * promosyonlar[promosyonIndex].indirim_orani;
        sepet->toplam_tutar -= indirimMiktari;
        printf("Promosyon uygulandi! Indirim miktari: %.2f TL\n", indirimMiktari);
    } else {
        printf("\t\t\t\tGeçersiz promosyon kodu!\n");
    }
}


void restoran_puanla() {
    int secilen_index;
    float yiyecek_puani, hizmet_puani, atmosfer_puani, ortalama_puan;
    char yeni_puan, yorum_yazmak;
    
    printf("\t\t\t\t\tLütfen asagidaki restoranlardan birini seçin:\n");
    for (int i = 0; i < RESTORAN_SAYISI; ++i) {
        printf("\t\t\t\t\t\t%d) %s\n", restaurants[i].id, restaurants[i].name);
    }
    printf("\t\t\t\t\t\tSeçiminiz (1-%d): ", RESTORAN_SAYISI);
    scanf("%d", &secilen_index);
    
    if (secilen_index >= 1 && secilen_index <= RESTORAN_SAYISI) {
        secilen_index -= 1;
        if (restaurants[secilen_index].puanlar[0] != 0) {
            printf("\t\t\t\t\tZaten bu restorana puan verdiniz.\n");
            printf("\t\t\t\t\tEski puaniniz: %.2f\n", restaurants[secilen_index].puanlar[0]);
            printf("\t\t\t\t\tYeni puaninizi girin: ");
            scanf("%f", &restaurants[secilen_index].puanlar[0]);
            printf("\t\t\t\t\tPuaniniz basariyla güncellendi.\n");
            return;
        }
        printf("\t\t\t\t\tYiyecek için puaninizi giriniz (0-5 arasi): ");
        scanf("%f", &yiyecek_puani);
        printf("\t\t\t\t\tHizmet için puaninizi giriniz (0-5 arasi): ");
        scanf("%f", &hizmet_puani);
        printf("\t\t\t\t\tAtmosfer için puaninizi giriniz (0-5 arasi): ");
        scanf("%f", &atmosfer_puani);
        
        if (yiyecek_puani < 0 || yiyecek_puani > 5 || hizmet_puani < 0 || hizmet_puani > 5 || atmosfer_puani < 0 || atmosfer_puani > 5) {
            printf("\t\t\t\t\tGeçersiz puan girdiniz. Puanlar 0 ile 5 arasinda olmalidir.\n");
            return;
        }
        
        ortalama_puan = (yiyecek_puani + hizmet_puani + atmosfer_puani) / 3;
        restaurants[secilen_index].puanlar[0] = ortalama_puan;
        
        printf("\t\t\t\t\tRestoranin puani: %.2f\n", ortalama_puan);
        
        printf("\t\t\t\t\tVerdiginiz puanlar cevaplara göredir bu puani begendiniz mi? (e/h): ");
        scanf(" %c", &yeni_puan);
        
        if (yeni_puan != 'e' && yeni_puan != 'E') {
            printf("\t\t\t\t\tYeni puaninizi girin: ");
            scanf("%f", &restaurants[secilen_index].puanlar[0]);
            printf("\t\t\t\t\tPuaniniz basariyla güncellendi.\n");
        }
        
        printf("\t\t\t\t\tYorum yapmak ister misiniz? (e/h): ");
        scanf(" %c", &yorum_yazmak);
        
        if (yorum_yazmak == 'e' || yorum_yazmak == 'E') {
            printf("\t\t\t\t\tYorumunuzu yazin: ");
            scanf(" %[^\n]s", restaurants[secilen_index].yorum);
            printf("\t\t\t\t\tYorumunuz basariyla kaydedildi.\n");
        }
        else if (yorum_yazmak != 'h' && yorum_yazmak != 'H') {
            printf("\t\t\t\t\tGeçersiz bir giris yaptiniz. Lütfen 'e' veya 'h' seklinde yanitlayin.\n");
        }
    }
    else {
        printf("\t\t\t\t\tGeçersiz bir seçim yaptiniz. Lütfen 1 ile %d arasinda bir sayi girin.\n", RESTORAN_SAYISI);
    }
}


void yorum_sil() {
    int secilen_index;
    char emin_misiniz;
    
    printf("\t\t\t\t\t\tLütfen silmek istediginiz restoranin yorumunu seçin:\n");
    for (int i = 0; i < RESTORAN_SAYISI; ++i) {
        printf("\t\t\t\t\t\t%d) %s\n", restaurants[i].id, restaurants[i].name);  
    }
    printf("\t\t\t\t\t\tSeçiminiz (1-5): ");
    scanf("%d", &secilen_index);
    
    if (secilen_index >= 1 && secilen_index <= RESTORAN_SAYISI) {
        secilen_index -= 1;  
        if (restaurants[secilen_index].yorum[0] != '\0') {  
            printf("\t\t\t\t\t\t%s restoraninin yorumlari:\n%s\n", restaurants[secilen_index].name, restaurants[secilen_index].yorum);
            printf("\t\t\t\t\t\tBu yorumu silmek istediginize emin misiniz? (e/h): ");
            scanf(" %c", &emin_misiniz);  
            
            if (emin_misiniz == 'e' || emin_misiniz == 'E') {
                restaurants[secilen_index].yorum[0] = '\0';  
                printf("\t\t\t\t\t\t\tYorum basariyla silindi.\n");
            } else if (emin_misiniz != 'h' && emin_misiniz != 'H') {
                printf("\t\t\t\t\t\tGeçersiz bir giris yaptiniz. Lütfen 'e' veya 'h' seklinde yanitlayin.\n");
            }
        } else {
            printf("\t\t\t\t\t\tBu restoran için henüz bir yorum bulunmamaktadir.\n");
        }
    } else {
        printf("\t\t\t\t\t\tGeçersiz bir seçim yaptiniz. Lütfen 1 ile 5 arasinda bir sayi girin.\n");
    }
}



void yorumlari_gor() {
    int secilen_index;
    float ortalama_puan;
    
    printf("\t\t\t\t\tHangi restoranin yorumlarini görmek istersiniz?\n");
    for (int i = 0; i < RESTORAN_SAYISI; ++i) {
        printf("\t\t\t\t\t\t%d) %s\n", restaurants[i].id, restaurants[i].name); 
    }
    printf("\t\t\t\t\t\tSeçiminiz (1-5): ");
    scanf("%d", &secilen_index);
    
    if (secilen_index >= 1 && secilen_index <= RESTORAN_SAYISI) {
        secilen_index -= 1;
        
        if (restaurants[secilen_index].puanlar[0] != 0) {
            printf("\t\t\t\t\tRestoranin ortalama puani: %.2f\n", restaurants[secilen_index].puanlar[0]);
        }
        else {
            printf("\t\t\t\t\tBu restoran için henüz bir puanlama yapilmamistir.\n");
        }
        
        if (restaurants[secilen_index].yorum[0] != '\0') {
            printf("\t\t\t\t\tRestorana gelen yorumlar:\n\t\t\t%s\n", restaurants[secilen_index].yorum);
        }
        else {
            printf("\t\t\t\t\tBu restoran için henüz bir yorum bulunmamaktadir.\n");
        }
    }
    else {
        printf("\t\t\t\tGeçersiz bir seçim yaptiniz. Lütfen 1 ile 5 arasinda bir sayi girin.\n");
    }
}


void menu_goster() {
    char secim;
	printf("\n\t\t\t\t\t\t    RESTORAN DEGERLENDIRME  \n");
    printf("\t\t\t\t\t\t1) Restorani Puanla ve Yorum Yap\n");
    printf("\t\t\t\t\t\t2) Yorumu Sil\n");
    printf("\t\t\t\t\t\t3) Yorumlari Gör\n");
    printf("\t\t\t\t\t\t4) Geri dön\n");
    printf("\t\t\t\t\t\t-----------------------------------\n");
    printf("\t\t\t\t\t\tSeciminiz: ");
    scanf(" %c", &secim);
      
        switch (secim) {
            case '1':
                restoran_puanla();
                break;
            case '2':
                yorum_sil();
                break;
            case '3':
                yorumlari_gor();
                break;
            case '4':
            	break;
			default:
                printf("Geçersiz bir seçim yaptiniz. Lütfen tekrar deneyin.\n");
                break;
        }
    
}


void tabloBasla() {
	srand(time(NULL));
	for (int i = 0; i < 5; i++){
		for (int j = 0; j < 10; j++){
			restaurants[i].masalar[j] = rand() % 2;
		}
	}
}


void restaurant_status(int restaurant_index) {
    int bos = 0;
    for (int i = 0; i < 10; i++) {
        if (restaurants[restaurant_index].masalar[i] == 0) {
            bos++;
        }
    }
    printf("\t\t\t\t\tRestoran %s, ", restaurants[restaurant_index].name);
    if (bos > 5) {
        printf("\t\t\t\t\tsu anda oldukça bos.\n");
    } 
	else {
        printf("\t\t\t\t\tsu anda oldukça kalabalik.\n");
    }
}


void make_reservation() {
    //system("cls");
	tabloBasla();
	puts("--------------------------------------------------------------------------------------------");
	printf("\t\tHos geldiniz! Lütfen rezervasyon yapmak istediginiz restorani seçiniz\n");
	puts("--------------------------------------------------------------------------------------------");
    for (int i = 0; i < 5; i++) {
        printf("\t\t\t\t\t%d) %s\n", restaurants[i].id, restaurants[i].name);
    }
    printf("\t\t\t\t\t6) Geri dön\n");

    int choice;
    printf("\t\t\t\t\tSeçiminiz: ");
    scanf("%d", &choice);

    if (choice == 6) {
        return;
    }

    if (choice < 1 || choice > 5) {
        printf("\t\t\t\tGeçersiz seçim.\n");
        make_reservation();
        return;
    }
    
    int chosen_restaurant_index = choice - 1;;
    restaurant_status(chosen_restaurant_index);

    char purpose[20];
    printf("\t\t\t\tNe için rezervasyon yapmak istiyorsunuz?\n\t\t\t\t1) Evlilik Teklifi\n\t\t\t\t2) Nikah\n\t\t\t\t3) Etkinlik\n\t\t\t\t4) Diger\n\t\t\t\t5) Geri dön\n\t\t\t\tSeçiminiz: ");
    scanf("%s", purpose);

    if (strcmp(purpose, "5") == 0) {
        make_reservation();
        return;
    } 
	else if (atoi(purpose) < 1 || atoi(purpose) > 4) {
        printf("\t\t\t\tGeçersiz seçim.\n");
        make_reservation();
        return;
    }

    int num_people;
    printf("\t\t\t\tKaç kisi için rezervasyon yapmak istiyorsunuz? (En fazla 50 kisi): ");
    scanf("%d", &num_people);

    if (num_people <= 0 || num_people > 50) {
        printf("\t\t\t\t---------------------------\n");
		printf("\t\t\t\tGeçersiz kisi sayisi.\n");
		printf("\t\t\t\t---------------------------\n");
        make_reservation();
        return;
    }

    int day_choice;
    printf("\t\t\t\t----------------------------------\n");
	printf("\t\t\t\t\t\tHangi gün gelmek istersiniz?\n\t\t\t\t\t\t1) Pazartesi\n \t\t\t\t\t\t2) Sali\n \t\t\t\t\t\t3) Çarsamba\n\t\t\t\t\t\t4) Persembe\n\t\t\t\t\t\t5) Cuma\n\t\t\t\t\t\t6) Cumartesi\n\t\t\t\t\t\t7) Pazar\n\t\t\t\t\t\tSeçiminiz: ");
	scanf("%d", &day_choice);

    if (day_choice < 1 || day_choice > 7) {
        printf("\t\t\t\tGeçersiz gün seçimi.\n");
        make_reservation();
        return;
    }

    char reservation_time[10];
    printf("\t\t\t\t----------------------------------\n");
	printf("\t\t\t\tSaat kaçta rezervasyon yapmak istiyorsunuz? (Örnegin: 13:30) ");
	scanf("%s", reservation_time);

	printf("\t\t\t\t\t\tLütfen masa numarasi seçiniz:\n");
    chosen_restaurant_index = choice - 1; // Kullanicinin seçtigi restoranin indeksi
    for (int i = 0; i < 10; i++) {
        if (restaurants[chosen_restaurant_index].masalar[i] == 0) { // Eger masa bossa
            printf("\t\t\t\t\t\tMasa %d (Bos)\n", i + 1);
        } 
		else {
            printf("\t\t\t\t\t\tMasa %d (Dolu)\n", i + 1);
        }
    }
    int table_choice;
    printf("\t\t\t\t\t\tSeçiminiz: ");
    scanf("%d", &table_choice);
    while (table_choice < 1 || table_choice > 10 || restaurants[chosen_restaurant_index].masalar[table_choice - 1] == 1) {
        printf("\t\t\t\tGeçersiz veya dolu masa. Lütfen baska bir masa seçiniz: ");
        scanf("%d", &table_choice);
    }

    // Masa seçildi olarak isaretle
    restaurants[chosen_restaurant_index].masalar[table_choice - 1] = 1;

    // Rezervasyon bilgilerini göster
    printf("Rezervasyonunuz basariyla alinmistir!\n");

    printf("\t\t\t\tRezervasyonunuz basariyla alinmistir!\n");
    printf("\t\t\t\t--------------------------------------------\n");
	printf("\t\t\t\tRestoran: %s\n", restaurants[choice - 1].name);
    printf("\t\t\t\t-----------------------------------------------\n");
	printf("\t\t\t\tRezervasyon için amaç: %s\n", (strcmp(purpose, "1") == 0) ? "Evlilik" : (strcmp(purpose, "2") == 0) ? "Nikah" : 
	(strcmp(purpose, "3") == 0) ? "Etkinlik" : "Diger");
    printf("\t\t\t\t---------------------------------------------\n");
	printf("\t\t\t\tKisi sayisi: %d\n", num_people);
    printf("\t\t\t\t------------------------------------------------\n");
	printf("\t\t\t\tRezervasyon günü: %s\n", days[day_choice - 1]);
    printf("\t\t\t\t---------------------------------------------\n");
	printf("\t\t\t\tRezervasyon saati: %s\n", reservation_time);
	printf("\t\t\t\t--------------------------------------------\n");
	printf("\t\t\t\tMasa numariniz %d\n", table_choice);
	printf("\t\t\t\t--------------------------------------------");
}


// Sepete ürün ekleme fonksiyonu
void urunEkle(struct Sepet *sepet, struct Urun urun){
    sepet -> urunler[sepet -> urun_sayisi] = urun;
    sepet -> urun_sayisi++;
    sepet -> toplam_tutar += urun.fiyat * urun.adet;
}

// Sepetten ürün çikarma fonksiyonu
void urunCikar(struct Sepet *sepet, int index, int adet) {
    if (adet >= sepet->urunler[index].adet ){
        sepet -> toplam_tutar -= sepet -> urunler[index].fiyat * sepet -> urunler[index].adet;
        for (int i = index; i < sepet -> urun_sayisi - 1; i++) {
            sepet -> urunler[i] = sepet -> urunler[i + 1];
        }
        sepet -> urun_sayisi--;
    } 
	else { 
        sepet -> toplam_tutar -= sepet -> urunler[index].fiyat * adet;
        sepet->urunler[index].adet -= adet;
    }
}


// Menüyü yazdirma fonksiyonu
void menuyuYazdir() {
    //system("cls");
	char *kategoriAdlari[8] = {"1)Kahvalti", "2)Kebap", "3)Çorba", "4)Meze", "5)Salata", "6)Sicak Yemek", "7)Tatli", "8)Içecek"};

    printf("MENÜ:\n");
    printf("------------------------\n");
    for (int kategori = 0; kategori < 8; kategori++){
        printf("%s:\n", kategoriAdlari[kategori]);
        for (int i = 0; i < 8; i++) {
            printf("%d. %s\t%.2f TL\tStok: %d\n", i + 1, menu[kategori][i].ad, menu[kategori][i].fiyat, stoklar[kategori][i]);
            printf("\tMalzemeler: ");
            if (kategori == 0 && i == 0) 
                printf("Beyaz peynir, Kasar peynir, Yesil zeytin, Siyah zeytin domates ve salatalik");
            
            else if (kategori == 0 && i == 1) 
                printf("Bal, peynir, zeytin, domates, salatalik, reçel, Tereyag, Nutella, Yesil Biber, kasar peyniri, Aci ezme");
            
           else if (kategori == 0 && i == 2) 
                printf("2 adet Yumurta, tereyag, Kapya biber");
                
           else if (kategori == 0 && i == 3) 
                printf("2 adet yumurta, sucuk, tereyag");
                
          else if (kategori == 0 && i == 4) 
                printf("Çilek, Kivi, Muz , Ceviz, Nutella, beyaz çikolata, pankek sosu, pudra sekeri");
                
          else if (kategori == 0 && i == 5) 
                printf("Toz seker, maya, un");
          
		  else if (kategori == 0 && i == 6) 
                printf("Yesil biber, Domates, 3 adet yumurta, Terayag");
                
          else if (kategori == 0 && i == 7) 
                printf("4 adet yumurta, un");
                
		  else if (kategori == 1 && i == 0) 
                printf("Bulgur pilavi, Sogan, Domates, Köz biber, Lavas");
            
		  else if (kategori == 1 && i == 1) 
                printf("Bulgur pilavi, Sogan, Domates, Köz biber, Lavas");
                
          else if (kategori == 1 && i == 2) 
                printf("Bulgur pilavi, Sogan, Domates, Köz biber, tirnakli pide");
                
          else if (kategori == 1 && i == 3) 
                printf("Sumakli sogan");
                
        else if (kategori == 1 && i == 4) 
                printf("Közlenmis domates, Közlenmis Sivri biber, Bulgur pilavi, Sumakli sogan, Lavas");
            
        else if (kategori == 1 && i == 5) 
                printf("Pirinç pilavi, Biber, Domates, Tirnakli pide");
        
        else if (kategori == 1 && i == 6) 
                printf("Közlenmis biber, Pide");
        
        else if (kategori == 1 && i == 7) 
                printf("Pilav, Pide");
                
        else if (kategori == 2 && i == 0) 
                printf("Limon, Pul biber, Kitir ekmek");
        
		else if (kategori == 2 && i == 1) 
                printf("Kasar peyniri");
                
        else if (kategori == 2 && i == 2) 
                printf("Limon");
        
        else if (kategori == 2 && i == 3) 
                printf("Pide, Limon");
        
        else if (kategori == 2 && i == 4) 
                printf("Limon");
        
        else if (kategori == 2 && i == 5) 
                printf("Kizarmis ekmek");
        
        else if (kategori == 2 && i == 6) 
                printf("Ekmek");
        
        else if (kategori == 2 && i == 7) 
                printf("Ekmek");
                
        else if (kategori == 3 && i == 0) 
                printf("Domates, Sivri biber, Sarimsak, Sogan, Maydanoz, Biber salçasi, limon suyu, zeytinyag, nar eksisi, nane, kirmizi biber, Sumak, Isot");
        	
        else if (kategori == 3 && i == 1) 
                printf("yogurt, su, salata, Sarimsak, Nane");      
		
		else if (kategori == 3 && i == 2) 
                printf("Ezine beyaz peynir, Lor peynir, Feslegen, Antep fistigi,  Ceviz ici, Maydonaz, Sarimsak, Kekik");
                
        else if (kategori == 3 && i == 3) 
                printf("Nohut, Tahin, Limon suyu, sarimsak"); 
        
        else if (kategori == 3 && i == 4) 
                printf("Süzme yogurt, Nane, Sarimsak Zeytinyag, Dereotu, Tereyag, Beyaz peynir ");
                
        else if (kategori == 3 && i == 5) 
                printf("Zeyin, Maydonaoz, Sogan, Kapya biber");
                
        else if (kategori == 3 && i == 6) 
                printf("Patlican, Domates, Sivri biber, Sumak, Limon suyu");
            
        else if (kategori == 3 && i == 7) 
                printf("Etli Köfte, Lavas, Kivircik, Limon");
                
        else if (kategori == 4 && i == 0) 
                printf("Marul, Lahana, Havuç, Maydonoz, Dereotu");
                
        else if (kategori == 4 && i == 1) 
                printf("ince Kiyilmis Kuru Sogan, Yesil biber, Domates, Ceviz içi, Zeytinyag, Nar Eksisi");
                
        else if (kategori == 4 && i == 2) 
                printf("Salata, Viber, Domates, Maydonoz, Dereotu");
                
        else if (kategori == 4 && i == 3) 
                printf("Marul, Roka, Taze Sogan, Nane, Maydonoz, Dereotu, Nane");
        
        else if (kategori == 4 && i == 4) 
                printf("Domates, Yesil biber, Sogan, Maydonoz");
        
        else if (kategori == 4 && i == 5) 
                printf("yumurta, Worcestershire sos, ançuez, sarimsak, Dijon hardali, Parmesan peyniri");
        
        else if (kategori == 4 && i == 6) 
                printf("Patetes, Havuç, Bezelye, Salatalik tursusu, Dereotu, Misir");
                
        else if (kategori == 4 && i == 7) 
                printf("Patetes, Havuç, bezelye, Salatalik tursusu, Dereotu, Mayonez, Yogurt");
                
        else if (kategori == 5 && i == 0) 
                printf("Yogurt, Ekmek");
                
        else if (kategori == 5 && i == 1) 
                printf("Limon, Maydanoz");
        
        else if (kategori == 5 && i == 2) 
                printf("Limon, Maydanoz");
                
        else if (kategori == 5 && i == 3) 
                printf("Domates sosu, Yogurt, Limon, Maydonoz");
                
        else if (kategori == 5 && i == 4) 
                printf("Sarimsakli Yogurt");
        
        else if (kategori == 5 && i == 5) 
                printf("Maydanoz, Salatalik, Domates");
        
        else if (kategori == 5 && i == 6) 
                printf("Maydonoz, Domates, Salatalik");
                
        else if (kategori == 5 && i == 7) 
                printf("Limon, Maydonoz, Sogan");
                
        else if (kategori == 6 && i == 0) 
                printf("Antep fistigi, Kaymak");
                
        else if (kategori == 6 && i == 1) 
                printf("Antep Fistigi, Kaymak, Süt");
                
        else if (kategori == 6 && i == 2) 
                printf("Kaymak, Süt");
        
		else if (kategori == 6 && i == 3) 
                printf("Beyaz Dondurma");
                
        else if (kategori == 6 && i == 4) 
                printf("Kaymak");
		
		else if (kategori == 6 && i == 5) 
                printf("Süt");
			
		else if (kategori == 6 && i == 6) 
                printf("Dondurma ");
				
		else if (kategori == 6 && i == 7) 
                printf("Süt");
				
		else if (kategori == 7 && i == 0) 
                printf("");	
				
		
				printf("\n");
        }
        printf("------------------------\n");
    }
}

// Sepeti görüntüleme fonksiyonu
void sepetiGoruntule(struct Sepet *sepet){
    //system("cls");
	if (sepet->urun_sayisi == 0) {
        printf("\t\t\t\t\t\tSepetinizde bir sey yok. :(\n");
        return;
    }

    printf("\n\t\t\t\tSEPETINIZ:\n");
    printf("\t\t\t\t------------------------\n");
    for (int i = 0; i < sepet->urun_sayisi; i++){
        printf("\t\t\t\t%s\t\t%.2f TL\t\t%d adet\n", sepet->urunler[i].ad, sepet->urunler[i].fiyat, sepet->urunler[i].adet);
    }
    printf("\t\t\t\t------------------------\n");
    printf("\t\t\t\tToplam Tutar: %.2f TL\n", sepet->toplam_tutar);
}


int satisSayilari[8][8] = {0}; // Her kategori ve öge için satis sayilarini tutan dizi
void siparis_ver(struct Sepet *sepet) {
    int kategori, secim, adet, silinecek, cikarilacak_adet;
    char devam, devam_sil;

    do{
        menuyuYazdir();

        printf("\nSiparis vermek istediginiz kategoriyi seçin (9 Çikis): ");
        scanf("%d", &kategori);

        if(kategori == 9)
            return;

        printf("Siparis vermek istediginiz ögeyi seçin: ");
        scanf("%d", &secim);

        printf("Kaç adet istediginizi belirtin: ");
        scanf("%d", &adet);

        // Stoktan ürün düsürme
        if (adet > stoklar[kategori - 1][secim - 1]){
            printf("Stokta yeterli ürün yok!\n");
            continue;
        } 
		else {
            stoklar[kategori - 1][secim - 1] -= adet;
            satisSayilari[kategori - 1][secim - 1] += adet; // Satis sayisini güncelledim populerlik için 
        }

        struct Urun urun = {.adet = adet, .fiyat = menu[kategori - 1][secim - 1].fiyat, .stok = stoklar[kategori - 1][secim - 1]};
        strcpy(urun.ad, menu[kategori - 1][secim - 1].ad);

        urunEkle(sepet, urun);
        printf("------------------------------\n");
		printf("Siparisiniz alindi! Toplam Tutar: %.2f TL\n", sepet->toplam_tutar);

        printf("Devam etmek istiyor musunuz? (E/H): ");
        scanf(" %c", &devam); // Bosluk karakteri scanf'ten önceki tamponu temizler
    } 
	while (devam == 'E' || devam == 'e');
	
	    sepetiGoruntule(sepet); // Siparis verme islemi tamamlandiktan sonra sepeti görüntüle
}


void sepetSilme(struct Sepet *sepet) {
    int silinecek, cikarilacak_adet;
    char devam_sil;
    sepetiGoruntule(sepet);
    
    do {
        printf("Silmek istediginiz ögenin numarasini giriniz (1-%d) geri dönmek için 0 giriniz :=) : ", sepet->urun_sayisi);
        scanf("%d", &silinecek);
        if (silinecek == 0) {
            break; 
        }
        
        if(silinecek >= 1 && silinecek <= sepet->urun_sayisi){
            printf("Kaç adet çikarmak istiyorsunuz? (1-%d): ", sepet->urunler[silinecek - 1].adet);
            scanf("%d", &cikarilacak_adet);
            urunCikar(sepet, silinecek - 1, cikarilacak_adet);
            printf("%d adet öge basariyla silindi.\n", cikarilacak_adet);
            sepetiGoruntule(sepet);
        } else {
            printf("Geçersiz öge numarasi.\n");
        }

        printf("Baska bir öge silmek istiyor musunuz? (E/H): ");
        scanf(" %c", &devam_sil);
    } 
	while (devam_sil == 'E' || devam_sil == 'e');
}


void populerMenuyuYazdir(int satisSayilari[8][8], struct MenuItem menu[8][8], char *kategoriAdlari[]) {
    printf("\t\t\t\t\tPOPÜLER MENÜ ÖGELERI:\n");
    printf("\t\t\t\t\t------------------------\n");
    
    // Normal menü için en popüler ögeleri yazdir
    for (int kategori = 0; kategori < 8; kategori++) {
        int maxSatis = 0;
		for (int i = 0; i < 8; i++) {
            if (satisSayilari[kategori][i] > maxSatis) {
                maxSatis = satisSayilari[kategori][i];
            }
        }
        // Eger bir kategoride satis yapilmissa en popüler ögeyi bul ve yazdir
        if (maxSatis > 0) {
            for (int i = 0; i < 8; i++) {
                if (satisSayilari[kategori][i] == maxSatis) {
                    printf("\t\t\t\t\t%s - %s: %d satis\n", kategoriAdlari[kategori], menu[kategori][i].ad, satisSayilari[kategori][i]);
                    break; // Her kategoride en popüler olani sadece bir kez yazdir
                }
            }
        }
    }
}

//Sikça Sorulan Sorular
void sss(){
	int sorusecim;
	printf("----------------------Sikça Sorulan Sorular----------------------\n\n\n");
    printf("1) Nasil üye olabilirim?\n\n");
    printf("2) Daha önce üye olmustum nasil giris yapabilirim?\n\n");
    printf("3) Nasil siparis verebilirim?\n\n");
    printf("4) Rezervasyon yapmak istiyorum nasil yapabilirim?\n\n");
    printf("5) Sepetimdeki ürünleri ve siparis tutarimi nasil görüntüleyebilirim?\n\n");
    printf("6) Sepetime yanlis ürün ekledim nasil silebilirim?\n\n");
    printf("7) Ödeme seçenekleri neler ve nasil ödeme yapabilirim?\n\n");
    printf("8) Indirim kodumu kabul etmiyor?\n\n");
    printf("9) Saat kaça kadar siparis verebilirim?\n\n");
    printf("10) Indirim kuponu nasil kazanabilirim?\n\n");
    printf("11) Bilgilerimi dogru girdigimden emin degilim nasil kontrol edebilirim?\n\n");
    printf("12) Kart bilgimi vermek istemedigim durumda ödememi nasil yapabilirim?\n\n");
    printf("13) Siparisimle ilgili sorun yasiyorum sizlere nasil ulasabilirim?\n\n\n");
    
    while(1) { 
    	printf("Lütfen cevabini görmek istediginiz soru seçimini yapiniz (0 ile çikiniz): ");
    	scanf("%d", &sorusecim);
    
    	if(sorusecim == 0)
    	break; 
    
    
    switch(sorusecim){
		case 1:  
            printf("Kodu açtiginizda karsiniza çikan ilk ekranda 1'i tuslayarak isim, soyisim, kullanici adi, sifre, adres ve kart bilgilerinizi girerek basarili bir sekilde kayit olabilirsiniz.\n");
                break;
        case 2:
            printf("Giris ekraninda 2'yi tuslayip kullanici adiniz ve sifrenizi yazdiginizda giris yapacaktir.\n");
                break;
        case 3:
            printf("Hesap olusturup ya da kayit olduktan sonra çikan ekranda 2'yi tuslayarak siparis verme ekranini görebilirsiniz. Daha sonra sizden istenen siparis bilgilerinizi girdiginizde siparisiniz olusturulmaktadir.\n");
                break;
        case 4:
            printf("Hesap olusturduktan ya da hesabiniz varsa giris yaptiktan sonra çikan ekranda 3'ü tuslayarak rezervasyon yaptirabilirsiniz. Rezervasyon bilgilerinizi eksiksiz ve dogru girdiginizden emin olmalisiniz.\n");
                break;
        case 5:
            printf("Siparis verdikten sonra siparis ekranindan çikis yapip 4'ü tusladiginizda siparis tutarinzi ve sepetinizdeki ürünler görüntülenebilmektedir.\n");
                break;
        case 6:
            printf("Siparis verdikten sonra 6'yi tuslayarak sepetten ürün seçme bölümüne gidebilirsiniz. Buradan silmek istediginiz ürünü tuslama yöntemiyle basarili bir sekilde silebilirsiniz.\n");
                break;
        case 7: 
            printf("Ödeme ekranina 5'i tuslayarak gidebilirsiniz. Ödeme seçeneklerimiz ise nakit, kredi karti ve kripto paradir. Dogru tuslama yaptiginizdan emin olun.\n");
                break;
        case 8:
            printf("Menüden 7'yi tusladiktan sonra promosyon kismi görüntüleniyor, bu kisimda sizden istenen promosyon kodunuzu eksiksiz, dogru ve bosluksuz girdiginizden emin olun.\n");
                break;
        case 9:
            printf("Resoranimiz 'Her insanin her an acikmaya hakki vardir.' prensibinden yola çikarak hafta içi 09.00'dan 00.00'a kadar hafta sonlari ise saat 09.00'dan 04.00'a kadar açiktir.\n");
                break;
        case 10: 
            printf("Her ay belirli tarihlerde kuponalarimizi sizlere sunuyoruz. Bu kuponlari kaçirmamak için bizi takipte kalin.\n");
                break;
        case 11: 
            printf("Giris yaptiktan ya da kayit olduktan sonra bilgilerinizi dogru girdiginizden emin degilseniz giris menüsünde 1'i tuslayabilirsiniz.\n");
                break;
        case 12:
            printf("Siparis verdikten sonra ödeme kisminda nakit ödemeyi seçerseniz kart bilgilerinize gerek kalmadan ödeme yapabilirsiniz.\n");
                break;
        case 13:
            printf("Ana menüdeki seçeneklerden 8'i yani 'Hakkimizida' kismini tusladiginiz takdirde subelerimizin iletisim numaralari görüntülenebilmektedir.\n");
                break;
        default:
            printf("Hatali tuslama! Lütfen dogru tuslama yaptiginizdan emin olunuz!\n");
    }
  }
}


void tarih() {
    time_t timer; // time_t türünde bir degisken tanimliyoruz
    struct tm *zaman; // struct tm türünde bir isaretçi tanimliyoruz
    time(&timer); // time fonksiyonu ile timer degiskenine saniye cinsinden zamani atiyoruz
    zaman = localtime(&timer); // localtime fonksiyonu ile timer degiskenini yerel zamana göre struct tm yapisina çeviriyoruz
    printf("Tarih: %d.%d.%d\n", zaman->tm_mday, zaman->tm_mon + 1, zaman->tm_year + 1900); // struct tm yapisindaki tarih bilgilerini ekrana yazdiriyoruz
    printf("Saat: %d:%d:%d\n", zaman->tm_hour, zaman->tm_min, zaman->tm_sec); // struct tm yapisindaki saat bilgilerini ekrana yazdiriyoruz
}

void odeme_yap(struct User *user) {
    int secim;
    printf("\n\t\t\t\t\t\t--- Ödeme Yap ---\n");
    printf("\t\t\t\t\t\t1) Nakit         |\n");
    puts("\t\t\t\t\t\t------------------");
    printf("\t\t\t\t\t\t2) Kredi Karti   |\n");
    puts("\t\t\t\t\t\t------------------");
    printf("\t\t\t\t\t\t3) Kripto Para   |\n");
    puts("\t\t\t\t\t\t------------------");
    printf("\t\t\t\t\t\t4) Geri Dön      |\n");
    puts("\t\t\t\t\t\t------------------");
    printf("\t\t\t\t\t\tÖdeme yöntemini seçin: ");
    scanf("%d", &secim);

    switch (secim) {
        case 1:
            printf("Nakit ödeme yapildi.\n");
    printf("   ______    ______   __                        __                      __                               \n");
    printf("  /      \\  /      \\ |  \\                      |  \\                    |  \\                              \n");
    printf(" |  $$$$$$\\|  $$$$$$\\ \\$$ __    __   ______   _| $$_           ______  | $$  _______  __    __  _______  \n");
    printf(" | $$__| $$| $$_  \\$$|  \\|  \\  |  \\ /      \\ |   $$ \\         /      \\ | $$ /       \\|  \\  |  \\|       \\ \n");
    printf(" | $$    $$| $$ \\    | $$| $$  | $$|  $$$$$$\\ \\$$$$$$        |  $$$$$$\\| $$|  $$$$$$$| $$  | $$| $$$$$$$\\\n");
    printf(" | $$$$$$$$| $$$$    | $$| $$  | $$| $$    $$  | $$ __       | $$  | $$| $$ \\$$    \\ | $$  | $$| $$  | $$\n");
    printf(" | $$  | $$| $$      | $$| $$__/ $$| $$$$$$$$  | $$|  \\      | $$__/ $$| $$ _\\$$$$$$\\| $$__/ $$| $$  | $$\n");
    printf(" | $$  | $$| $$      | $$ \\$$    $$ \\$$     \\   \\$$  $$       \\$$    $$| $$|       $$ \\$$    $$| $$  | $$\n");
    printf("  \\$$   \\$$ \\$$       \\$$ _\\$$$$$$$  \\$$$$$$$    \\$$$$         \\$$$$$$  \\$$ \\$$$$$$$   \\$$$$$$  \\$$   \\$$\n");
    printf("                         |  \\__| $$                                                                         \n");
    printf("                          \\$$    $$                                                                         \n");
    printf("                           \\$$$$$$                                                                          \n");
            tarih();
            break;
        case 2:
            printf("Kredi karti ile ödeme yapildi.\n");
    printf("   ______    ______   __                        __                      __                               \n");
    printf("  /      \\  /      \\ |  \\                      |  \\                    |  \\                              \n");
    printf(" |  $$$$$$\\|  $$$$$$\\ \\$$ __    __   ______   _| $$_           ______  | $$  _______  __    __  _______  \n");
    printf(" | $$__| $$| $$_  \\$$|  \\|  \\  |  \\ /      \\ |   $$ \\         /      \\ | $$ /       \\|  \\  |  \\|       \\ \n");
    printf(" | $$    $$| $$ \\    | $$| $$  | $$|  $$$$$$\\ \\$$$$$$        |  $$$$$$\\| $$|  $$$$$$$| $$  | $$| $$$$$$$\\\n");
    printf(" | $$$$$$$$| $$$$    | $$| $$  | $$| $$    $$  | $$ __       | $$  | $$| $$ \\$$    \\ | $$  | $$| $$  | $$\n");
    printf(" | $$  | $$| $$      | $$| $$__/ $$| $$$$$$$$  | $$|  \\      | $$__/ $$| $$ _\\$$$$$$\\| $$__/ $$| $$  | $$\n");
    printf(" | $$  | $$| $$      | $$ \\$$    $$ \\$$     \\   \\$$  $$       \\$$    $$| $$|       $$ \\$$    $$| $$  | $$\n");
    printf("  \\$$   \\$$ \\$$       \\$$ _\\$$$$$$$  \\$$$$$$$    \\$$$$         \\$$$$$$  \\$$ \\$$$$$$$   \\$$$$$$  \\$$   \\$$\n");
    printf("                         |  \\__| $$                                                                         \n");
    printf("                          \\$$    $$                                                                         \n");
    printf("                           \\$$$$$$                                                                          \n");
            tarih();
            break;
        case 3:
            printf("Kripto para adreslerimiz.\n");
            printf("Bitcoin adresimiz: 1N5KUkUHhUuJ6RgVXa3s2Q3xbzGh8XxQh8 \n ");
            printf("Ethereum adresmiz: 0x8a03b9Cc34eC3B2AEd0b18Cec3Eba7e1c2A859F2 ");
    printf("   ______    ______   __                        __                      __                               \n");
    printf("  /      \\  /      \\ |  \\                      |  \\                    |  \\                              \n");
    printf(" |  $$$$$$\\|  $$$$$$\\ \\$$ __    __   ______   _| $$_           ______  | $$  _______  __    __  _______  \n");
    printf(" | $$__| $$| $$_  \\$$|  \\|  \\  |  \\ /      \\ |   $$ \\         /      \\ | $$ /       \\|  \\  |  \\|       \\ \n");
    printf(" | $$    $$| $$ \\    | $$| $$  | $$|  $$$$$$\\ \\$$$$$$        |  $$$$$$\\| $$|  $$$$$$$| $$  | $$| $$$$$$$\\\n");
    printf(" | $$$$$$$$| $$$$    | $$| $$  | $$| $$    $$  | $$ __       | $$  | $$| $$ \\$$    \\ | $$  | $$| $$  | $$\n");
    printf(" | $$  | $$| $$      | $$| $$__/ $$| $$$$$$$$  | $$|  \\      | $$__/ $$| $$ _\\$$$$$$\\| $$__/ $$| $$  | $$\n");
    printf(" | $$  | $$| $$      | $$ \\$$    $$ \\$$     \\   \\$$  $$       \\$$    $$| $$|       $$ \\$$    $$| $$  | $$\n");
    printf("  \\$$   \\$$ \\$$       \\$$ _\\$$$$$$$  \\$$$$$$$    \\$$$$         \\$$$$$$  \\$$ \\$$$$$$$   \\$$$$$$  \\$$   \\$$\n");
    printf("                         |  \\__| $$                                                                         \n");
    printf("                          \\$$    $$                                                                         \n");
    printf("                           \\$$$$$$                                                                          \n");
           tarih();
           break;
    	case 4: 
    printf("\t\t\t\t\t\tGeri Döndürüliyorsunuz...");
    	break;	
		default:
            printf("Geçersiz seçenek! Lütfen tekrar deneyin.\n");
        odeme_yap(user); 
		break;
        
    }
}


void hakkinda() {
    
	printf ("                     \n\t\t\t\t\t\t<<<Hakkimizda>>>\n");
    printf("\t\t\t<<<---ISTANBUL'DAKI KALITELI YEMEKLERI EN IYI LEZZETLERLE SIZLERE SUNUYORUZ--->>>\t\n\n");
    printf("\t\t\t\t                 ---Çalisma Saatleri---\n");
    printf ("\t\t\t\t           Pazartesi - Cuma:  09.00 - 00.00\n");
    printf ("\t\t\t\t          Cumartesi - Pazar:  09.00 - 04.00\n");
    printf ("\n\n\t\t\t\t\t  <<---Diger Subelerimiz--->>\n");
    printf("\t\t\t\t    --- Subelerimiz ---              --- Iletisim ---      \n");
    printf("\t\t\t\t   Istanbul/Küçükçekmece ---------> +90 (123)-456-7890     \n");
    printf("\t\t\t\t   Istanbul/Besiktas -------------> +90 (123)-456-7890    \n");
    printf("\t\t\t\t   Istanbul/Sultangazi -----------> +90 (123)-456-7890     \n");
    printf("\t\t\t\t   Istanbul/Basaksehir -----------> +90 (123)-456-7890     \n");
    printf("\t\t\t\t   Istanbul/Tuzla ----------------> +90 (123)-456-7890    \n");
    printf("\t\t\t--------------------------------------------------------------------\n");
	printf("\t\t\t\t\t\tPromosyon kodlarimiz\n");
	printf("\t\t\t--------------------------------------------------------------------\n");
	printf("\t\t\t\t\t\t\tYENI20\n");
	printf("\t\t\t--------------------------------------------------------------------\n");
	printf("\t\t\t\t\t\t\tINDIRIM50\n");
    printf("\t\t\t--------------------------------------------------------------------\n");
    printf("\t\t\t\t\t\t\tBILALINAN\n");
    printf("\t\t\t--------------------------------------------------------------------\n");
}


void animation(const char *text, unsigned int delay_ms) {
	for (int i = 0; text[i] != '\0'; i++) {
		putchar(text[i]);
		fflush(stdout);
		usleep(delay_ms * 1000);
	}
}



int main(int argc, char *argv[]) {	
	int choice;
	struct Sepet sepet = {.urun_sayisi = 0, .toplam_tutar = 0};
	char *kategoriAdlari[8] = {"1)Kahvalti", "2)Kebap", "3)Çorba", "4)Meze", "5)Salata", "6)Sicak Yemek", "7)Tatli", "8)Içecek"};
	setlocale(LC_ALL, "Turkish");

	animation("\n ____                                                                                   ___          /|        ",1);
    animation("\n|  _ \\ ___  ___| |_ ___  _ __ __ _ _ __  _ _ __ ___  _ ______ _            ||||     .-\"`   `\"-.     | |  __    ",1);
    animation("\n| |_) / _ \\/ __| __/ _ \\| '__/ _` | '_ \\| | '_ ` _ \\| |_  / _` |      |||| ||||   .'  .-'`'-.  '.   | | /  \\   ",1);
    animation("\n|  _ <  __/\\__ \\ || (_) | | | (_| | | | | | | | | | | |/ / (_| |      \\  /  ||  /  ;           ;  \\  \\| \\  /   ",1);
    animation("\n|_| \\_\\___||___/\\__\\___/|_|  \\__,_|_| |_|_|_| |_| |_|_/___\\__,_|       ||   ||  | ;             ; |  ||  ||    ",1);
    animation("\n| |__   ___  ___  __ _  ___| | __| (_)_ __ (_)____                     ||   ||  | ;             ; |  ||  ||    ",1);
    animation("\n| '_ \\ / _ \\/ __|/ _` |/ _ \\ |/ _` | | '_ \\| |_  /                     ||   ||  \\  ;           ;  /  ||  ||    ",1);
    animation("\n| | | | (_) \\__ \\ (_| |  __/ | (_| | | | | | |/ /                      ||   ||   \\  '.       .'  /   ||  ||     ",1);
    animation("\n|_| |_|\\___/|___/\\__, |\\___|_|\\__,_|_|_| |_|_/___                      ||   ||    \".  `-.,.-'  .\"    ||  ||     ",1);
    animation("\n             )_)  |___/                                                ||   ||      '-.,___,.-'      ||  ||     ",1);
    animation("\n                                                                                                                 ",1);

	 	    
        
struct User user;
    do {
    	//system("cls"); //bu cls hemen siliyor 
		printf("\n\t\t\t\t\t\t-- Kullanici Islemleri --\n");
        puts("\t\t\t\t\t\t--------------------------");
		printf("\t\t\t\t\t\t| 1) Kullanici Kaydi     |\n");
        puts("\t\t\t\t\t\t--------------------------");
        printf("\t\t\t\t\t\t| 2) Kullanici Giris     |\n");
        puts("\t\t\t\t\t\t--------------------------");
		printf("\t\t\t\t\t\t| 3) S.S.S               |\n");
        puts("\t\t\t\t\t\t--------------------------");
        printf("\t\t\t\t\t\t| 4) Cikis               |\n");
        puts("\t\t\t\t\t\t--------------------------");
		printf("\t\t\t\t\t\t|Seciminiz: ");
        scanf("%d", &choice);
    	        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser(&user);
                if (checkCredentials(&user)) {
                    int submenu_choice;
                 do {
						printUserMenu();
                        scanf("%d", &submenu_choice);
                        switch (submenu_choice) {
                            case 1:
                                system("cls");
								printUserInfo(&user);
                                printf("1)Bilgilerimi Güncelle\n2)Bilgilerim dogru\nSeciminiz: ");
                                scanf("%d", &submenu_choice);
                                if (submenu_choice == 1) {
                                	updateUserInfo(&user);
								}
								else if (submenu_choice == 2) {
									printf("Bilgilerim dogru");
								}
								else {
									printf("Geçersiz seçenek\n");
								}
                                break;
                            case 2:
                                system("cls");
								siparis_ver(&sepet);
                                break;
                            case 3:
                                system("cls");
								make_reservation();
                                break;
                            case 4:
                                system("cls");
								sepetiGoruntule(&sepet);
                                break;
                            case 5:
                                system("cls");
								sepetiGoruntule(&sepet);
								odeme_yap(&user);
                                break;
                            case 6:
                                system("cls");
								sepetSilme(&sepet);
                                break;
                            case 7:
                                    system("cls");
									puts("\t\t\t\t-------------------------------------------");
									printf("\t\t\t\tPromosyon kodunu girin eger yoksa(0)giriniz: ");
                                    char promosyonKodu[20];
                                    scanf("%s", promosyonKodu);
                                    uygulaPromosyon(&sepet, promosyonKodu);
                                break;
                            case 8:                            
								system("cls");
								hakkinda();					
                                break;
                            case 9:
								system("cls");
								menu_goster(); 
                            	break;
							case 10:
								system("cls");
								populerMenuyuYazdir(satisSayilari, menu, kategoriAdlari);
                                break;
                            case 11:
                            	//v_siparis_ver(&sepet); //bu kod düzgün çalismiyor 
                            	printf("\t\t\t\t\t\tGiris Ekranina yönlediriliyorsunuz\n");
								break;							
							default:
                                printf("\t\t\t\t\t\tGecersiz secim! Lutfen tekrar deneyin.\n");
                        }
                    } 
					while (submenu_choice != 11);
                } 
				else {
                    printf("Giris basarisiz. Kullanici adi veya sifre hatali.\n");
                }
                break;
            case 3:
            	system("cls");
				sss(); 
            	break;
			case 4:
				printf("\t\t\t\t\t\tCikis yapiliyor...\n");
                  printf ("\t\t\t\t\t\t *****       ***** \n");
  				  printf ("\t\t\t\t\t        *******     ******* \n");
  				  printf ("\t\t\t\t\t       *********   ********** \n");
  				  printf ("\t\t\t\t\t       ********************** \n");
  				  printf ("\t\t\t\t\t        ******************** \n");
  				  printf ("\t\t\t\t\t         ****************** \n ");
  				  printf ("\t\t\t\t\t          **************** \n");
  				  printf ("\t\t\t\t\t           ************** \n");
  				  printf ("\t\t\t\t\t            ************ \n");
  				  printf ("\t\t\t\t\t             ********** \n");
  				  printf ("\t\t\t\t\t              ******** \n");
  				  printf ("\t\t\t\t\t               ****** \n");
  				  printf ("\t\t\t\t\t                **** \n");
  				  printf ("\t\t\t\t\t                 ** \n");
                break;
            default:
                printf("\t\t\t\t\t\tGecersiz secim! Lutfen tekrar deneyin.\n");
        }	  	
	}
	while (choice != 4); 
	return 0;
}
