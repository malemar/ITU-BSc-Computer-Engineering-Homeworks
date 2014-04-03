ödevin tamamýný yapamamýþ bulunmaktayýz ancak küçük bir kýsmýný yapabildik.

Öncelikle veri yapýlarýný tanýtalým:

struct message {
	char *to, *data;
};

struct msgbox_dev {
	struct message* messages;
	unsigned msg_count;
	struct cdev cdev;
};

Ýletilen mesajlarýn tutulacaðý bir message struct'ý. Eðer projeyi
tamamlayabilseydik içinde yalnýzca gönderilen ve data deðil 
mesajý yollayan da olacak, mesajýn okunup okunmadýðýný belirten
bir deðiþken daha olacaktý.

ikinci veri yapýsý device'ý tanýmlayan veri yapýsý. Sorun tam olarak
bu veri yapýsýný iþin içine dahil edince baþlýyor. Þu anda baþarýlan iþ
þunlardan oluþmakta:

Device'a " to User: Hello user! " þeklinde echo komutuyla bir þey
yazýldýðýnda driver bu yazýlanýn "User" kýsmýný char *to deðiþkeninde,
"Hello user!" kýsmýný char *data deðiþkeninde tutuyor. cat komutuyla
device okunduðunda ise "Hello user!" stringi karþýmýza çýkýyor.

Eðer msgbox_dev struct'ýný da dahil edebilseydik, yazýlan her echo'yu
dev'in messages arrayinde tutacaktýk. Ekrana bastýrýrken de döngü kullanýp
kaç tane mesaj kaydedilmiþse hepsini ekrana basabilecektik. Þu an en son 
kaydedilen mesaj ekrana basýlýyor. Eðer tüm mesajlarý ekrana bastýrabilme
aþamasýna gelebilseydik mesajýn kimden geldiði bilgisini de veri yapýsýna 
ekleyip, mesajýn okunup okunmadýðý bilgisini veri yapýsýna girip,
messagebox'un çalýþma modunu belirleyen (EXCLUDE_READ , INCLUDE_READ)
flag'leri de göz önünde bulundurarak doðru bir þekilde mesajlarý
ekrana bastýrabilecek, daha sonra bir de ioctl fonksiyonu yazarak
bu bayraklarýn deðerlerini deðiþtirebilecektik.