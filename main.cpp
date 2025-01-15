#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>

class Stack {
private:
    int* stack;
    int top;
    int capacity;

public:
    explicit Stack(int capacity) : top(-1), capacity(capacity) {
        stack = new int[capacity];
    }

    bool isEmpty() const {
        return top == -1;
    }
    bool isFull() const {
        return top == capacity - 1;
    }

    void push(int value) {
        if (isFull()) return;
        stack[++top] = value;
    }
    int pop() {
        if (isEmpty()) return -1;
        return stack[top--];
    }

    ~Stack() {
        delete[] stack;
    }
};
class Queue {
private:
    int* queue;
    int front;
    int rear;
    int size;
    int capacity;

public:
    explicit Queue(int capacity) : front(0), rear(0), size(0), capacity(capacity) {
        queue = new int[capacity];
    }

    bool isEmpty() const {
        return size == 0;
    }
    int getSize() const {
        return this->size;
    }
    int getFront() const {
        if (size == 0) {
            return -1;
        }
        return queue[front];
    }

    void push(int value) {
        if (size == capacity) {
            return;
        }
        queue[rear] = value;
        rear = (rear + 1) % capacity;
        size++;
    }
    void pop() {
        if (isEmpty()) return; 
        front = (front + 1) % capacity; 
        size--; 
    }

    ~Queue() {
        delete[] queue;
    }

};

class Graph {
private:
	int order = 0;  
    int* neighbours = nullptr; 
    int** vertexes = nullptr;  
     
    int componentsNumber = 0;  
    int* componentsSizes = nullptr;
    int** components = nullptr;

    static void merge(int* arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        int* leftArray = new int[n1];
        int* rightArray = new int[n2];

        for (int i = 0; i < n1; i++) {
            leftArray[i] = arr[left + i];
        }
        for (int j = 0; j < n2; j++) {
            rightArray[j] = arr[mid + 1 + j];
        }

        int i = 0, j = 0;

        int k = left;
        while (i < n1 && j < n2) {
            if (leftArray[i] <= rightArray[j]) {
                arr[k] = leftArray[i];
                i++;
            }
            else {
                arr[k] = rightArray[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            arr[k] = leftArray[i];
            i++;
            k++;
        }

        while (j < n2) {
            arr[k] = rightArray[j];
            j++;
            k++;
        }

        delete[] leftArray;
        delete[] rightArray;
    }
    void mergeSort(int* arr, int left, int right) const {
        if (left < right) {
            int mid = left + (right - left) / 2;

            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);

            merge(arr, left, mid, right);
        }
    }
    void degreeSequence() {
        int* neighboursCopy = new int[order];
        for (int i = 0; i < order; i++) {
            neighboursCopy[i] = neighbours[i];
        }
        mergeSort(neighboursCopy, 0, order - 1);

        for (int i = order - 1; i >= 0; i--) {
            printf("%d ", neighboursCopy[i]);
        }
        printf("\n");

        delete[] neighboursCopy;
    }

    void numberOfComponents() {
        bool* visited = new bool[order];
        for (int i = 0; i < order; i++) {
            visited[i] = false;
        }

        for (int i = 0; i < order; i++) {
            if (!visited[i]) {
                Stack stack(order);
                stack.push(i);  
                visited[i] = true;
                
                while (!stack.isEmpty()) { 
                    int top = stack.pop(); 

                    for (int j = 0; j < neighbours[top]; j++) { 
                        int neighbour = vertexes[top][j] - 1;  
                        if (!visited[neighbour]) { 
                            stack.push(neighbour);  
                            visited[neighbour] = true; 
                        }
                    }
                }
                componentsNumber++;
            }
        }

        printf("%d\n", componentsNumber);
        delete[] visited;
    }   

    bool bipartitenessHelper(int number, int color, int* colors) {
        colors[number] = color;

        for (int i = 0; i < neighbours[number]; i++) {
            int neighbour = vertexes[number][i] - 1;

            if (colors[neighbour] == -1) {
                if (!bipartitenessHelper(neighbour, 1 - color, colors)) {
                    return false;
                }
            }
            else if (colors[neighbour] == color) {
                return false;
            }
        }
        return true;
    }
    void bipartiteness() {
        int* colors = new int[order];
        for (int i = 0; i < order; i++) {
            colors[i] = -1;
        }

        for (int i = 0; i < order; i++) {
            if (colors[i] == -1) {
                if (!bipartitenessHelper(i, 0, colors)) {
                    printf("F\n"); 
                    delete[] colors;
                    return; 
                }
            }
        }

        printf("T\n");
        delete[] colors;
    }
   
    void eccentricityOfVertices() {
        int* visited = new int[order];
        for (int i = 0; i < order; i++) {
            visited[i] = -1;
        } 
        
        for (int vertex = 0; vertex < order; vertex++) {
            Queue q(order);
            q.push(vertex);
            visited[vertex] = vertex;

            int checked = 1;
            int distance = -1;
            while (!q.isEmpty()) {
                int size = q.getSize();
                for (int i = 0; i < size; i++) {
                    int v = q.getFront();
                    q.pop();

                    for (int j = 0; j < neighbours[v]; j++) {
                        int neighbour = vertexes[v][j] - 1;
                        if (visited[neighbour] != vertex) {
                            if (++checked == order) {
                                while (!q.isEmpty()) q.pop();
                                break;
                            }
                            q.push(neighbour);
                            visited[neighbour] = vertex;
                        }
                    }
                    if (checked == order) {
                        distance++;
                        break;
                    }
                }
                distance++;
            }
            printf("%d ", distance);
        }

        printf("\n"); 
        delete[] visited;
    }     

    static void planarity() {
        printf("?\n"); 
    }

    void verticesColoursGreedy() const {
        int* color = new int[order]; 
        int* used = new int[order]; 

        for (int i = 0; i < order; i++) { 
            color[i] = 0;
            used[i] = 0;  
        }
        color[0] = 1;

        for (int i = 1; i < order; i++) {
            for (int j = 0; j < neighbours[i]; j++) {
                int neighbour = vertexes[i][j] - 1; 
                if (color[neighbour]) { 
                    used[color[neighbour] - 1] = i;
                }
            }

            for (int j = 0; j < order; j++) {
                if (used[j] != i) {
                    color[i] = j + 1; 
                    break;
                }
            }
        }

        for (int i = 0; i < order; i++) {
            printf("%d ", color[i]);
        }
        printf("\n");

        delete[] color;
        delete[] used;
    }

    static void merge(int* degrees, int* vertices, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        int* leftDegrees = new int[n1];
        int* rightDegrees = new int[n2];
        int* leftVertices = new int[n1];
        int* rightVertices = new int[n2];

        for (int i = 0; i < n1; i++) {
            leftDegrees[i] = degrees[left + i];
            leftVertices[i] = vertices[left + i];
        }
        for (int j = 0; j < n2; j++) {
            rightDegrees[j] = degrees[mid + 1 + j];
            rightVertices[j] = vertices[mid + 1 + j];
        }

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (leftDegrees[i] >= rightDegrees[j]) {
                degrees[k] = leftDegrees[i];
                vertices[k] = leftVertices[i];
                i++;
            }
            else {
                degrees[k] = rightDegrees[j];
                vertices[k] = rightVertices[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            degrees[k] = leftDegrees[i];
            vertices[k] = leftVertices[i];
            i++;
            k++;
        }

        while (j < n2) {
            degrees[k] = rightDegrees[j];
            vertices[k] = rightVertices[j];
            j++;
            k++;
        }

        delete[] leftDegrees;
        delete[] rightDegrees;
        delete[] leftVertices;
        delete[] rightVertices;
    }
    void mergeSort(int* degrees, int* vertices, int left, int right) const {
        if (left < right) {
            int mid = left + (right - left) / 2;

            mergeSort(degrees, vertices, left, mid);
            mergeSort(degrees, vertices, mid + 1, right);

            merge(degrees, vertices, left, mid, right);
        }
    }
    void verticesColoursLF() {
        int* color = new int[order]; 
        int* used = new int[order];
        int* degrees = new int[order];
        int* sortedVertices = new int[order];

        for (int i = 0; i < order; i++) {
            color[i] = 0;
            used[i] = -1; 
            degrees[i] = neighbours[i]; 
            sortedVertices[i] = i; 
        }

        mergeSort(degrees, sortedVertices, 0, order - 1); 
        color[sortedVertices[0]] = 1; 

        for (int i = 1; i < order; i++) {
            int v = sortedVertices[i];

            for (int j = 0; j < neighbours[v]; j++) {
                int neighbour = vertexes[v][j] - 1;
                if (color[neighbour]) {
                    used[color[neighbour] - 1] = i;
                }
            }

            for (int j = 0; j < order; j++) {
                if (used[j] != i) {
                    color[v] = j + 1;
                    break;
                }
            }
        } 

        for (int i = 0; i < order; i++) {
            printf("%d ", color[i]);
        }
        printf("\n");

        delete[] color;
        delete[] used;
        delete[] degrees;
        delete[] sortedVertices;
    }

    void componentsInit() {
        int count = 0;
        bool* visited = new bool[order];
        for (int i = 0; i < order; i++) {
            visited[i] = false;
        }

        componentsSizes = new int[componentsNumber];
        components = new int* [componentsNumber];

        for (int i = 0; i < order; i++) {
            if (!visited[i]) {
                Stack stack(order);
                stack.push(i);
                visited[i] = true;

                int size = 0;
                while (!stack.isEmpty()) {
                    int top = stack.pop();
                    size++;

                    for (int j = 0; j < neighbours[top]; j++) {
                        int neighbour = vertexes[top][j] - 1;
                        if (!visited[neighbour]) {
                            stack.push(neighbour);
                            visited[neighbour] = true;
                        }
                    }
                }
                componentsSizes[count] = size;
                components[count] = new int[size];
                count++;
            }
        }

        count = 0;
        for (int i = 0; i < order; i++) {
            visited[i] = false;
        }

        for (int i = 0; i < order; i++) {
            if (!visited[i]) {
                Stack stack(order);
                stack.push(i);
                visited[i] = true;

                int number = 0;
                while (!stack.isEmpty()) {
                    int top = stack.pop();
                    components[count][number] = top;
                    number++;

                    for (int j = 0; j < neighbours[top]; j++) {
                        int neighbour = vertexes[top][j] - 1;
                        if (!visited[neighbour]) {
                            stack.push(neighbour);
                            visited[neighbour] = true;
                        }
                    }
                }
                count++;
            }
        }

        delete[] visited;
    }
    void verticesColoursSLF() { 
        componentsInit(); 

        int* color = new int[order];
        int* used = new int[order]; 
        int* saturation = new int[order];
        for (int i = 0; i < order; i++) {
            color[i] = 0;
            used[i] = -1;
            saturation[i] = 0;
        } 

        for (int c = 0; c < componentsNumber; c++) {
            int size = componentsSizes[c]; 
            if (size == 1) {
                color[components[c][0]] = 1;
                continue;
            }

            for (int i = 0; i < size; i++) {
                int iVertex = components[c][i];

                int chosen = -1;
                for (int j = 0; j < size; j++) {
                    int jVertex = components[c][j];
                    if (color[jVertex] == 0) {
                        if (chosen != -1) {
                            if (saturation[jVertex] > saturation[chosen]) {
                                chosen = jVertex;
                            }
                            else if (saturation[jVertex] == saturation[chosen]) {
                                if (neighbours[jVertex] > neighbours[chosen]) {
                                    chosen = jVertex;
                                }
                                else if (neighbours[jVertex] == neighbours[chosen]) {
                                    if (jVertex < chosen) {
                                        chosen = jVertex;
                                    }
                                }
                            }
                        }
                        else {
                            chosen = jVertex;
                        }
                    }
                }

                for (int j = 0; j < neighbours[chosen]; j++) {
                    int neighbour = vertexes[chosen][j] - 1;
                    if (color[neighbour] != 0) {
                        used[color[neighbour] - 1] = iVertex;
                    }
                }

                for (int j = 0; j < order; j++) { 
                    if (used[j] != iVertex) { 
                        color[chosen] = j + 1;  
                        break; 
                    }
                }

                for (int j = 0; j < neighbours[chosen]; j++) {
                    int neighbour1 = vertexes[chosen][j] - 1;
                    if (color[neighbour1] == 0) {
                        bool check = false;
                        for (int n = 0; n < neighbours[neighbour1]; n++) {
                            int neighbour2 = vertexes[neighbour1][n] - 1;
                            if (color[neighbour2] == color[chosen] && neighbour2 != chosen) {
                                check = true;
                                break;
                            }
                        }
                        if (!check) saturation[neighbour1]++;
                    }
                }
            }
        }
        
        for (int i = 0; i < order; i++) {
            printf("%d ", color[i]);
        }
        printf("\n");

        delete[] color; 
        delete[] used; 
        delete[] saturation;  
    }

    void differentC4Subgraphs() const {
        long long count = 0;

        for (int first = 0; first < order - 1; first++) {
            if (neighbours[first] <= 1) continue;

            bool* firstNeighbours = new bool[order];
            for (int i = 0; i < order; i++) {
                firstNeighbours[i] = false;
            }
            for (int i = 0; i < neighbours[first]; i++) {
                firstNeighbours[vertexes[first][i] - 1] = true;
            }

            for (int second = first + 1; second < order; second++) {
                if (neighbours[second] <= 1) continue; 
                int common = 0;
                for (int i = 0; i < neighbours[second]; i++) {
                    if (firstNeighbours[vertexes[second][i] - 1]) {
                        common++; 
                    }
                }
               count += common * (common - 1) / 2;
            }
            delete[] firstNeighbours;
        }

        printf("%lld\n", count / 2);  
    }

    void complementsEdges() const {
        int edges = 0;
        for (int i = 0; i < order; i++) {
            edges += neighbours[i];
        }
        edges /= 2; 

        long long complementEdges = (long long) order * (order - 1) / 2 - edges;
        printf("%lld\n", complementEdges);  
    }

public:
	Graph() {
		scanf("%d", &order);
        neighbours = new int[order]; 
        vertexes = new int*[order]; 

		for (int i = 0; i < order; i++) {
			scanf("%d", &neighbours[i]);
            vertexes[i] = new int[neighbours[i]];

            for (int j = 0; j < neighbours[i]; j++) {
                scanf("%d", &vertexes[i][j]);
            }
		}
	}
	void graphService() {
        degreeSequence(); 
        numberOfComponents();
        bipartiteness(); 
        eccentricityOfVertices();  
        planarity();
        verticesColoursGreedy(); 
        verticesColoursLF();
        verticesColoursSLF();
        differentC4Subgraphs();
        complementsEdges();
	}
    ~Graph() {
        for (int i = 0; i < order; i++) {
            delete[] vertexes[i];
        }
        delete[] neighbours;
        delete[] vertexes;

        for (int i = 0; i < componentsNumber; i++) {
            delete[] components[i];
        }
        delete[] componentsSizes;
        delete[] components;
	}

};

int main() {
	int graphsNumber = 0;
	scanf("%d", &graphsNumber);

	for (int i = 0; i < graphsNumber; i++) {
		Graph graph;
		graph.graphService();
	}

	return 0;
}