# 🚀 API Gateway Priority Request Scheduler

Bu proje, modern ağ sistemlerinin (API Gateway) trafik yönetimi problemini çözmek amacıyla C dilinde geliştirilmiş, **RFC 9213 (Extensible HTTP Priorities)** standardına tam uyumlu bir öncelikli kuyruk (Priority Queue) simülasyonudur.

Sistem, saniyede binlerce isteğin geldiği bir senaryoda sıradan veri yapılarının yaratacağı darboğazları (bottleneck) aşmak için **Min-Heap** veri yapısı kullanılarak optimize edilmiştir.

## 🌟 Öne Çıkan Özellikler

* **RFC 9213 Uyumluluğu:** Gelen ham HTTP header metinlerindeki `urgency` (0-7) ve `incremental` parametrelerini güvenli bir şekilde ayrıştırır.
* **Yüksek Performans (Min-Heap):** İstekleri geliş sırasına göre değil, aciliyet sırasına göre O(log N) karmaşıklığı ile işler.
* **Modüler Yazılım Mimarisi:** Spagetti koddan kaçınılarak fonksiyonlar `header (.h)` ve `source (.c)` dosyalarına izole edilmiştir.
* **Sıfır Bellek Sızıntısı (Zero Memory Leak):** Dinamik dizi boyutu (`malloc`/`realloc`) ve tam bellek temizliği (`free`) standartlarına harfiyen uyulmuştur.

---

## 🏗️ Modüler Mimari

Proje 3 ana katmandan oluşmaktadır:

1.  **Ayrıştırma Katmanı (`rfc_parser.h / .c`):** Sisteme giren string formatındaki istekleri (Örn: `Priority: u=1, i`) okur ve C yapılarına (struct) dönüştürür. Eksik verilerde varsayılan (u=3) güvenlik ağını çalıştırır.
2.  **Veri Yapısı Katmanı (`min_heap.h / .c`):** İş mantığından tamamen izole edilmiş öncelik kuyruğu motorudur. Yukarı ve aşağı kaydırma (heapify) işlemlerini yönetir.
3.  **Uygulama Katmanı (`main.c`):** Sistemi başlatan, kapasiteyi belirleyen ve diğer iki modülü birbiriyle haberleştiren ana kontrolcüdür.

---

## 📊 Neden Min-Heap? (Algoritmik Analiz)

Milyonlarca isteğin olduğu bir API Gateway sisteminde zaman karmaşıklığı hayati önem taşır:

| İşlem | Sırasız Dizi | Sıralı Dizi | Min-Heap (Bu Proje) |
| :--- | :--- | :--- | :--- |
| **Yeni İstek Ekleme** | O(1) | O(N) | **O(log N)** |
| **En Acili Çıkarma** | O(N) | O(1) | **O(log N)** |

* **Dizi Problemi:** Sıradan bir dizide en acil isteği bulmak için tüm diziyi taramak gerekir (O(N)). Bu, yüksek trafikte sistemin çökmesine neden olur.
* **Çözüm:** İkili Ağaç (Binary Tree) tabanlı Min-Heap algoritması, okuma ve yazma maliyetlerini logaritmik seviyede tutarak sistemi her iki senaryoda da stabil kılar.

---

## 💻 Kurulum ve Çalıştırma

Projede standart C kütüphaneleri (stdio.h, stdlib.h, string.h, stdbool.h) kullanılmıştır. Harici bir bağımlılık yoktur.

**Terminal (GCC) üzerinden derlemek için:**
```bash
gcc main.c min_heap.c rfc_parser.c -o api_gateway
./api_gateway
