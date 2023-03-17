#include "all.c"

const int N = 2e6;
struct gsllist **AdjLists;
struct hset *Vertices, *VertexCover, *T;
int NumberOfNodes, NumberOfEdges, SizeOfVertexCover;

/*
Hàm Get_Line đọc một dòng từ file đầu vào, lưu vào một xâu.
Xóa bỏ kí tự xuống dòng ở cuối xâu và in xâu ra màn hình.
*/
void Get_Line(char *line, FILE *file) {
    fgets(line, 1024, file);
    line[strlen(line) - 1] = '\0';
    printf("%s\n", line);
}

/*
Hàm Parse_Input: tải các ipnut vào bộ nhớ 
    - Đầu vào: file lưu đồ thị, file lưu tập đỉnh cần kiểm tra bao phủ.
    - Đầu ra:
        + Đồ thị: tập đỉnh (V), tập cạnh (E)
        + Tập đỉnh kiểm tra bao phủ (VC)
        + Tập đỉnh T: phục vụ cho thuật toán kiểm tra tập đỉnh bao phủ
            T = V \ VC
*/
void Parse_Input(char *graph, char *vc) {
    // Bước 1: Tải đồ thị
    printf("Tải đồ thị vào bộ nhớ...\n");
    FILE *Graph = fopen(graph, "r");
    /* 
    Xử lý 4 dòng đầu chứa thông tin (loại đồ thị, tên, số đỉnh, số cạnh) của đồ thị.
    Tách thông tin về số đỉnh và số cạnh của đồ thị ở dòng thứ 3 và lưu lại.
    */
    char line[1024];
    for (int i = 0; i < 3; i++) {
        Get_Line(line, Graph);
    }
    const char s[2] = " ";
    char *token[10];
    int i = 0;
    token[i] = strtok(line, s);
    while(token[i] != NULL) {
        i++;
        token[i] = strtok(NULL, s);
    }
    NumberOfNodes = atoi(token[2]);
    NumberOfEdges = atoi(token[4]);
    Get_Line(line, Graph);

    Vertices = hset_create(gtype_hash_l, gtype_cmp_l, NULL);
    AdjLists = calloc(N, sizeof(struct gsllist *));
    for(int i = 0; i < N; i++) {
        AdjLists[i] = gsl_create_list(NULL);
    }

    int source, sink;
    while (fscanf(Graph, "%d %d\n", &source, &sink) != EOF) {
        hset_insert(Vertices, gtype_l(source));        
        gsl_push_back(AdjLists[source], gtype_l(sink));
    }

    fclose(Graph);
    printf("Đã tải đồ thị xong\n\n");

    // Bước 2: Tải tập đỉnh để kiểm tra bao phủ
    printf("Tải tập đỉnh cần kiểm tra bao phủ vào bộ nhớ...\n");
    FILE *VC = fopen(vc, "r");
    /* 
    Xử lý 3 dòng đầu chứa thông tin chung. Tách thông tin về số đỉnh trong tập đỉnh và lưu lại.
    */
    for (int i = 0; i < 3; i++) {
        Get_Line(line, Graph);
    }
    i = 0;
    token[i] = strtok(line, s);
    while(token[i] != NULL) {
        i++;
        token[i] = strtok(NULL, s);
    }
    SizeOfVertexCover = atoi(token[5]);

    VertexCover = hset_create(gtype_hash_l, gtype_cmp_l, NULL);

    char c;
    int vertex;
    while (fscanf(VC, "%c %d\n", &c, &vertex) != EOF) {
        hset_insert(VertexCover, gtype_l(vertex));
    }
    
    fclose(VC);
    printf("Đã tải tập đỉnh xong\n\n");

    // Bước 3: Tạo tập T = V \ VC
    T = hset_create(gtype_hash_l, gtype_cmp_l, NULL);
    hset_traverse(key, Vertices) {
        if (hset_index_of(VertexCover, gtype_l(key->l)) == -1) {
            hset_insert(T, gtype_l(key->l));
        }
    }
}

/*
Hàm BFS: thực hiện duyệt đồ thị theo chiều rộng 
    - Đầu vào:
        + Điểm mà từ đó bắt đầu duyệt
        + Danh sách kề của mỗi đỉnh
        + Mảng đánh dấu các đỉnh đã thăm
    - Đầu ra:
        + In ra màn hình thứ tự thăm các đỉnh
*/
void BFS(int startVertex, bool visited[]){
    int order = 0;
    struct gsllist *queue = gsl_create_list(NULL);
    visited[startVertex] = true;
    gsl_push_back(queue, gtype_l(startVertex));
    while (!gsl_is_empty(queue)) {
        int currentVertex = gsl_front_value(queue)->l;
        gsl_pop_front(queue);
        order++;
        printf("%d. Visited %d\n", order, currentVertex);

        gsl_traverse(cur, AdjLists[currentVertex]) {
            if (!visited[cur->l]) {
                visited[cur->l] = true;
                gsl_push_back(queue, gtype_l(cur->l));
            }
        }
    }
    gsl_free(queue);
}

/*
Hàm DFS: thực hiện duyệt đồ thị theo chiều sâu 
    - Đầu vào:
        + Điểm mà từ đó bắt đầu duyệt
        + Danh sách kề của mỗi đỉnh
        + Mảng đánh dấu các đỉnh đã thăm
    - Đầu ra:
        + In ra màn hình thứ tự thăm các đỉnh
*/
void DFS(int startVertex, bool visited[]){
    int order = 0;
    struct gsllist *stack = gsl_create_list(NULL);
    visited[startVertex] = true;
    gsl_push_front(stack, gtype_l(startVertex));
    while (!gsl_is_empty(stack)) {
        int currentVertex = gsl_front_value(stack)->l;
        gsl_pop_front(stack);
        order++;
        printf("%d. Visited %d\n", order, currentVertex);

        gsl_traverse(cur, AdjLists[currentVertex]) {
            if (!visited[cur->l]) {
                visited[cur->l] = true;
                gsl_push_front(stack, gtype_l(cur->l));
            }
        }
    }
    gsl_free(stack);
}

/*
Hàm Check_Vertex_Cover: Kiểm tra tập đỉnh đã cho có bao phủ đồ thị tương ứng không
    - Đầu vào:
        + Tập đỉnh T đã tạo ở trên
        + Danh sách kề của mỗi đỉnh
    - Đầu ra:
        + 1 nếu tập đỉnh bao phủ đồ thị
        + 0 nếu tập đỉnh không bao phủ đồ thị
*/
int Check_Vertex_Cover() {
    hset_traverse(key, T) {
        int CurrentVertex = (int) key->l;
        struct gsllist *CurrentAdjList = AdjLists[CurrentVertex];
        gsl_traverse(cur, CurrentAdjList) {
            int AdjNode = (int) cur->l;
            if (hset_index_of(T, gtype_l(AdjNode)) != -1) {
                return 0;
            }
        }
    }
    return 1;
}

/*
Hàm Free: Giải phóng tất cả bộ nhớ đã cấp phát
*/
void Free(){
    for (int i = 0; i < N; i++) {
        gsl_free(AdjLists[i]);
    }
    free(AdjLists);
    hset_free(Vertices);
    hset_free(VertexCover);
    hset_free(T);
}

void menu(){
    printf("***********************************************\n");
    printf("Chương trình thực hiện các thao tác trên đồ thị\n");
    printf("Chọn thao tác bạn muốn thực hiện\n");
    printf("1. Duyệt đồ thị theo thuật toán BFS\n");
    printf("2. Duyệt đồ thị theo thuật toán DFS\n");
    printf("3. Kiểm tra tập đỉnh bao phủ\n");
    printf("4. Thoát\n");
    printf("Lựa chọn của bạn là [1-4]: ");
}

int main(int argc, char *argv[]) {
    bool visited[N];
    Parse_Input(argv[1], argv[2]);
    int choice = -1;
    do {
        menu();
        scanf("%d",&choice);
        if(choice == 1){
            memset(&visited, false, sizeof(visited)); // Set tất cả các phần tử mảng đánh dấu visited = false, 
                                                      // tức chưa đỉnh nào được duyệt
            int startVertex;
            printf("Nhập đỉnh bắt đầu: ");
            scanf("%d", &startVertex);
            BFS(startVertex, visited);
        }
        else if(choice == 2){
            memset(&visited, false, sizeof(visited)); // Set tất cả các phần tử mảng đánh dấu visited = false, 
                                                      // tức chưa đỉnh nào được duyệt
            int startVertex;
            printf("Nhập đỉnh bắt đầu: ");
            scanf("%d", &startVertex);
            DFS(startVertex, visited);
        }
        else if(choice == 3){
            int isVertexCover = Check_Vertex_Cover();
            if(isVertexCover){
                printf("%s\n", "Là tập đỉnh bao phủ");
            } else {
                printf("%s\n", "Không là tập đỉnh bao phủ");
            }
        }
    } while(choice != 4);

    Free();
    return 0;
}