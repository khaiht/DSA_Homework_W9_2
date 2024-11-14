#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// cau truc node de luu tru thong tin ve file
typedef struct Node {
    char* name;             // ten file
    size_t size;            // kich thuoc file
    time_t modifiedTime;    // thoi gian sua doi cuoi cung
    struct Node* next;      // con tro toi node tiep theo
} Node;

// khoi tao mot node moi
Node* createNode(const char* fileName, size_t fileSize, time_t modTime) {
    Node* newNode = (Node*)malloc(sizeof(Node));   // cap phat bo nho cho node moi
    newNode->name = strdup(fileName);              // sao chep ten file
    newNode->size = fileSize;                      // luu kich thuoc file
    newNode->modifiedTime = modTime;               // luu thoi gian sua doi
    newNode->next = NULL;                          // khoi tao con tro next la NULL
    return newNode;
}

// cau truc de quan ly danh sach cac file
typedef struct {
    Node* start;   // con tro toi node dau tien trong danh sach
} FileCollection;

// khoi tao danh sach file
void initFileCollection(FileCollection* collection) {
    collection->start = NULL;   // khoi tao danh sach trong
}

// them file vao danh sach theo thu tu thoi gian sua doi
void addFileToList(FileCollection* collection, const char* fileName, size_t fileSize, time_t modTime) {
    Node* newNode = createNode(fileName, fileSize, modTime);   // tao node moi

    if (!collection->start || difftime(collection->start->modifiedTime, modTime) > 0) {
        // neu danh sach trong hoac file moi co thoi gian sua doi som hon file dau tien
        newNode->next = collection->start;
        collection->start = newNode;
    } else {
        Node* current = collection->start;
        // tim vi tri de chen file moi vao danh sach theo thu tu thoi gian
        while (current->next && difftime(current->next->modifiedTime, modTime) < 0) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

// tinh tong kich thuoc cua tat ca cac file trong danh sach
size_t totalFileSize(const FileCollection* collection) {
    size_t total = 0;   // bien tong kich thuoc file
    Node* current = collection->start;   // bat dau tu node dau tien
    while (current) {
        total += current->size;   // cong kich thuoc file hien tai vao tong
        current = current->next;  // di chuyen den node tiep theo
    }
    return total;   // tra ve tong kich thuoc file
}

// gioi han tong kich thuoc cua danh sach file
void limitTotalSize(FileCollection* collection, size_t maxSize) {
    while (totalFileSize(collection) > maxSize) {
        Node* smallest = collection->start;
        Node* prevSmallest = NULL;
        Node* current = collection->start;
        Node* previous = NULL;

        // tim file co kich thuoc nho nhat trong danh sach
        while (current) {
            if (current->size < smallest->size) {
                smallest = current;
                prevSmallest = previous;
            }
            previous = current;
            current = current->next;
        }

        // xoa file nho nhat khoi danh sach
        if (prevSmallest) {
            prevSmallest->next = smallest->next;
        } else {
            collection->start = smallest->next;
        }
        free(smallest->name);   // giai phong bo nho ten file
        free(smallest);         // giai phong bo nho node
    }
}

// hien thi danh sach cac file
void showFiles(const FileCollection* collection) {
    Node* current = collection->start;   // bat dau tu node dau tien
    while (current) {
        printf("File: %s, Kich co: %zu bytes, Lan cuoi: %s", 
               current->name, current->size, ctime(&current->modifiedTime));   // in thong tin file
        current = current->next;   // di chuyen den node tiep theo
    }
}

// sao chep file va them vao danh sach
void copyAndAddToCollection(FileCollection* collection, const char* src, const char* dest, size_t fileSize, time_t modTime) {
    #ifdef _WIN32
        char command[256];
        snprintf(command, sizeof(command), "copy %s %s", src, dest);   // tao lenh sao chep tren windows
        system(command);   // thuc hien lenh sao chep file
    #else
        char command[256];
        snprintf(command, sizeof(command), "cp %s %s", src, dest);   // tao lenh sao chep tren linux/mac
        system(command);   // thuc hien lenh sao chep file
    #endif
    addFileToList(collection, dest, fileSize, modTime);   // them file vao danh sach
}

// giai phong bo nho danh sach file
void freeFileCollection(FileCollection* collection) {
    Node* current = collection->start;   // bat dau tu node dau tien
    while (current) {
        Node* temp = current;            // luu lai node hien tai
        current = current->next;         // di chuyen den node tiep theo
        free(temp->name);   // giai phong bo nho ten file
        free(temp);         // giai phong bo nho node
    }
    collection->start = NULL;   // dat danh sach ve trong
}

int main() {
    FileCollection files;   // khoi tao danh sach file
    initFileCollection(&files);   // khoi tao danh sach trong

    // them cac file vao danh sach
    addFileToList(&files, "file1.txt", 1000, time(NULL) - 100);
    addFileToList(&files, "file2.txt", 2000, time(NULL) - 200);
    addFileToList(&files, "file3.txt", 500, time(NULL) - 50);

    printf("Cac file trong list\n");
    showFiles(&files);   // hien thi cac file trong danh sach

    // sao chep file va them vao danh sach
    copyAndAddToCollection(&files, "source_file.txt", "destination_file.txt", 1500, time(NULL) - 150);

    printf("\nCac file sau khi copy:\n");
    showFiles(&files);   // hien thi danh sach sau khi sao chep

    // gioi han tong kich thuoc cua danh sach file
    limitTotalSize(&files, (size_t)32 * 1024 * 1024 * 1024);

    printf("\nCac file sau khi gioi han 32MB:\n");
    showFiles(&files);   // hien thi danh sach sau khi gioi han kich thuoc

    // giai phong bo nho
    freeFileCollection(&files);

    return 0;
}