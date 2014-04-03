�devin tamam�n� yapamam�� bulunmaktay�z ancak k���k bir k�sm�n� yapabildik.

�ncelikle veri yap�lar�n� tan�tal�m:

struct message {
	char *to, *data;
};

struct msgbox_dev {
	struct message* messages;
	unsigned msg_count;
	struct cdev cdev;
};

�letilen mesajlar�n tutulaca�� bir message struct'�. E�er projeyi
tamamlayabilseydik i�inde yaln�zca g�nderilen ve data de�il 
mesaj� yollayan da olacak, mesaj�n okunup okunmad���n� belirten
bir de�i�ken daha olacakt�.

ikinci veri yap�s� device'� tan�mlayan veri yap�s�. Sorun tam olarak
bu veri yap�s�n� i�in i�ine dahil edince ba�l�yor. �u anda ba�ar�lan i�
�unlardan olu�makta:

Device'a " to User: Hello user! " �eklinde echo komutuyla bir �ey
yaz�ld���nda driver bu yaz�lan�n "User" k�sm�n� char *to de�i�keninde,
"Hello user!" k�sm�n� char *data de�i�keninde tutuyor. cat komutuyla
device okundu�unda ise "Hello user!" stringi kar��m�za ��k�yor.

E�er msgbox_dev struct'�n� da dahil edebilseydik, yaz�lan her echo'yu
dev'in messages arrayinde tutacakt�k. Ekrana bast�r�rken de d�ng� kullan�p
ka� tane mesaj kaydedilmi�se hepsini ekrana basabilecektik. �u an en son 
kaydedilen mesaj ekrana bas�l�yor. E�er t�m mesajlar� ekrana bast�rabilme
a�amas�na gelebilseydik mesaj�n kimden geldi�i bilgisini de veri yap�s�na 
ekleyip, mesaj�n okunup okunmad��� bilgisini veri yap�s�na girip,
messagebox'un �al��ma modunu belirleyen (EXCLUDE_READ , INCLUDE_READ)
flag'leri de g�z �n�nde bulundurarak do�ru bir �ekilde mesajlar�
ekrana bast�rabilecek, daha sonra bir de ioctl fonksiyonu yazarak
bu bayraklar�n de�erlerini de�i�tirebilecektik.