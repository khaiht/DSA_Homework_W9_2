#include <stdio.h>    // thu vien cho cac thao tac nhap xuat
#include <stdlib.h>   // thu vien cho cac thao tac cap phat bo nho dong
#include <string.h>   // thu vien cho cac thao tac xu ly chuoi

// cau truc node de luu tru moi tu va so lan xuat hien cua no
typedef struct WordNode {
    char* word;              // chuoi luu tru tu
    int count;               // bien dem so lan xuat hien cua tu
    struct WordNode* next;   // con tro toi node tiep theo trong danh sach
} WordNode;

// tao mot node moi
WordNode* createNode(const char* word) {
    WordNode* newNode = (WordNode*)malloc(sizeof(WordNode));  // cap phat bo nho cho node moi
    newNode->word = strdup(word);     // sao chep tu vao bien word
    newNode->count = 1;               // dat bien dem so lan xuat hien ban dau la 1
    newNode->next = NULL;             // node moi chua co node ke tiep
    return newNode;
}

// danh sach lien ket don de quan ly cac tu
typedef struct {
    WordNode* head;    // con tro toi node dau tien trong danh sach
} WordList;

// khoi tao danh sach
void initWordList(WordList* list) {
    list->head = NULL;   // dat con tro head bang NULL vi danh sach dang trong
}

// them mot tu vao danh sach hoac cap nhat so lan xuat hien
void addWord(WordList* list, const char* word) {
    WordNode* current = list->head;    // bat dau tu node dau tien
    WordNode* previous = NULL;         // node truoc node hien tai
    
    while (current != NULL) {   // duyet qua danh sach
        if (strcmp(current->word, word) == 0) {  // neu tu da ton tai
            current->count++;    // tang bien dem len 1
            return;
        }
        previous = current;   // luu lai node hien tai
        current = current->next;   // di chuyen den node tiep theo
    }
    
    WordNode* newNode = createNode(word);  // tao node moi neu tu chua ton tai
    if (previous != NULL) {
        previous->next = newNode;   // gan node moi vao sau node cuoi cung
    } else {
        list->head = newNode;   // neu danh sach trong, node moi la node dau tien
    }
}

// hien thi danh sach cac tu va so lan xuat hien cua chung
void showList(const WordList* list) {
    WordNode* current = list->head;   // bat dau tu node dau tien
    while (current != NULL) {   // duyet qua danh sach
        printf("word: %s, count: %d\n", current->word, current->count);   // in tu va so lan xuat hien
        current = current->next;   // di chuyen den node tiep theo
    }
}

// tim tu xuat hien nhieu nhat
char* mostCommonWord(const WordList* list) {
    WordNode* current = list->head;   // bat dau tu node dau tien
    char* frequentWord = NULL;        // bien luu tu xuat hien nhieu nhat
    int highestCount = 0;             // bien luu so lan xuat hien cao nhat
    
    while (current != NULL) {   // duyet qua danh sach
        if (current->count > highestCount) {   // neu so lan xuat hien lon hon so cao nhat hien tai
            highestCount = current->count;     // cap nhat so cao nhat
            frequentWord = current->word;      // cap nhat tu xuat hien nhieu nhat
        }
        current = current->next;   // di chuyen den node tiep theo
    }
    return frequentWord;
}

// xoa cac tu lap lai (vi du: "haha" hoac "mama")
void removeRepeats(WordList* list) {
    WordNode* current = list->head;   // bat dau tu node dau tien
    WordNode* previous = NULL;        // node truoc node hien tai
    
    while (current != NULL) {
        size_t len = strlen(current->word);   // lay do dai cua tu
        if (len >= 4 && 
            strncmp(current->word, current->word + len / 2, len / 2) == 0) {  // kiem tra xem tu co lap lai hay khong
            
            printf("removing reduplicative word: %s\n", current->word);   // in thong bao xoa tu
            
            if (previous != NULL) {
                previous->next = current->next;   // xoa node hien tai khoi danh sach
            } else {
                list->head = current->next;       // neu node dau tien can xoa
            }
            
            free(current->word);   // giai phong bo nho cua tu
            free(current);         // giai phong bo nho cua node
            current = (previous != NULL) ? previous->next : list->head;  // di chuyen den node tiep theo
        } else {
            previous = current;   // luu lai node hien tai
            current = current->next;   // di chuyen den node tiep theo
        }
    }
}

// dem tong so tu trong danh sach
int totalWords(const WordList* list) {
    int wordCount = 0;   // khoi tao bien dem so tu
    WordNode* current = list->head;   // bat dau tu node dau tien
    while (current != NULL) {   // duyet qua danh sach
        wordCount++;   // tang bien dem len 1
        current = current->next;   // di chuyen den node tiep theo
    }
    return wordCount;   // tra ve tong so tu
}

// giai phong bo nho danh sach
void freeWordList(WordList* list) {
    WordNode* current = list->head;   // bat dau tu node dau tien
    while (current != NULL) {   // duyet qua danh sach
        WordNode* temp = current;   // luu lai node hien tai
        current = current->next;    // di chuyen den node tiep theo
        free(temp->word);   // giai phong bo nho cua tu
        free(temp);          // giai phong bo nho cua node
    }
    list->head = NULL;   // dat head bang NULL vi danh sach da trong
}

int main() {
    WordList words;   // khoi tao danh sach tu
    initWordList(&words);   // khoi tao danh sach rong

    printf("enter a vietnamese sentence: ");   // nhap mot cau tieng viet
    char sentence[1024];   // mang de luu cau
    fgets(sentence, sizeof(sentence), stdin);   // doc cau tu nguoi dung nhap vao

    // loai bo ky tu xuong dong neu co
    sentence[strcspn(sentence, "\n")] = '\0';

    // tach cau thanh cac tu va them chung vao danh sach
    const char* delimiter = " ";   // ky tu phan cach la dau cach
    char* token = strtok(sentence, delimiter);   // tach tu dau tien
    
    while (token != NULL) {
        addWord(&words, token);   // them tu vao danh sach
        token = strtok(NULL, delimiter);   // lay tu tiep theo
    }

    printf("\ninitial word list:\n");
    showList(&words);   // hien thi danh sach cac tu

    char* commonWord = mostCommonWord(&words);   // tim tu xuat hien nhieu nhat
    if (commonWord != NULL) {
        printf("most frequent word: %s\n", commonWord);   // in tu xuat hien nhieu nhat
    }

    removeRepeats(&words);   // xoa cac tu lap lai
    printf("\nword list after removing repeats:\n");
    showList(&words);   // hien thi danh sach sau khi xoa

    int wordCount = totalWords(&words);   // dem tong so tu
    printf("total words: %d\n", wordCount);   // in tong so tu

    // giai phong bo nho
    freeWordList(&words);

    return 0;
}