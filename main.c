#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 20
#define COLS 40
#define MAX_SHAPES 100

// Canvas representing the 2D character grid
char canvas[ROWS][COLS];

// Shape types
typedef enum {
    SHAPE_LINE = 1,
    SHAPE_RECTANGLE,
    SHAPE_CIRCLE,
    SHAPE_TRIANGLE
} ShapeType;

// Shape parameter structures
typedef struct {
    int r1, c1;
    int r2, c2;
} LineParams;

typedef struct {
    int row, col;
    int width, height;
} RectParams;

typedef struct {
    int row, col;
    int radius;
} CircleParams;

typedef struct {
    int r1, c1;
    int r2, c2;
    int r3, c3;
} TriangleParams;

// Unified Shape structure
typedef struct {
    int id;
    ShapeType type;
    union {
        LineParams line;
        RectParams rect;
        CircleParams circle;
        TriangleParams triangle;
    } params;
} Shape;

// Array of active shapes and counters
Shape shapes[MAX_SHAPES];
int shape_count = 0;
int next_id = 1;

// Function declarations
void initializeCanvas();
void displayCanvas();
void drawLine(int r1, int c1, int r2, int c2);
void drawRectangle(int row, int col, int width, int height);
void drawCircle(int row, int col, int radius);
void drawTriangle(int r1, int c1, int r2, int c2, int r3, int c3);
void redrawCanvas();
void addShape();
void deleteShape();
void modifyShape();
void listShapes();
int getIntegerInput(const char* prompt);

void initializeCanvas()
{
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            canvas[i][j] = '_';
        }
    }
}

void displayCanvas()
{
    printf("\n");
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j++)
        {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
}

void drawLine(int r1, int c1, int r2, int c2)
{
    int dr = abs(r2 - r1);
    int dc = abs(c2 - c1);
    int sr = (r1 < r2) ? 1 : -1;
    int sc = (c1 < c2) ? 1 : -1;
    int err = dc - dr;

    while (1) {
        if (r1 >= 0 && r1 < ROWS && c1 >= 0 && c1 < COLS) {
            canvas[r1][c1] = '*';
        }
        if (r1 == r2 && c1 == c2) {
            break;
        }
        int e2 = 2 * err;
        if (e2 > -dr) {
            err -= dr;
            c1 += sc;
        }
        if (e2 < dc) {
            err += dc;
            r1 += sr;
        }
    }
}

void drawRectangle(int row, int col, int width, int height)
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int r = row + i;
            int c = col + j;
            if (r >= 0 && r < ROWS && c >= 0 && c < COLS) {
                if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                    canvas[r][c] = '*';
                }
            }
        }
    }
}

void drawCircle(int row, int col, int radius)
{
    if (radius < 0) return;
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    
    while (y >= x) {
        int points[8][2] = {
            {row + y, col + x},
            {row + y, col - x},
            {row - y, col + x},
            {row - y, col - x},
            {row + x, col + y},
            {row + x, col - y},
            {row - x, col + y},
            {row - x, col - y}
        };
        for (int i = 0; i < 8; i++) {
            int r = points[i][0];
            int c = points[i][1];
            if (r >= 0 && r < ROWS && c >= 0 && c < COLS) {
                canvas[r][c] = '*';
            }
        }
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

void drawTriangle(int r1, int c1, int r2, int c2, int r3, int c3)
{
    drawLine(r1, c1, r2, c2);
    drawLine(r2, c2, r3, c3);
    drawLine(r3, c3, r1, c1);
}

void redrawCanvas()
{
    initializeCanvas();
    for (int i = 0; i < shape_count; i++) {
        Shape s = shapes[i];
        switch (s.type) {
            case SHAPE_LINE:
                drawLine(s.params.line.r1, s.params.line.c1, s.params.line.r2, s.params.line.c2);
                break;
            case SHAPE_RECTANGLE:
                drawRectangle(s.params.rect.row, s.params.rect.col, s.params.rect.width, s.params.rect.height);
                break;
            case SHAPE_CIRCLE:
                drawCircle(s.params.circle.row, s.params.circle.col, s.params.circle.radius);
                break;
            case SHAPE_TRIANGLE:
                drawTriangle(s.params.triangle.r1, s.params.triangle.c1, s.params.triangle.r2, s.params.triangle.c2, s.params.triangle.r3, s.params.triangle.c3);
                break;
        }
    }
}

int getIntegerInput(const char* prompt)
{
    int val;
    char term;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d%c", &val, &term) == 2 && term == '\n') {
            return val;
        } else {
            // Clear buffer
            while (getchar() != '\n');
            printf("Invalid input. Please enter an integer.\n");
        }
    }
}

void listShapes()
{
    if (shape_count == 0) {
        printf("No objects in the picture.\n");
        return;
    }
    printf("\n--- Objects in Picture ---\n");
    for (int i = 0; i < shape_count; i++) {
        Shape s = shapes[i];
        printf("ID: %d | ", s.id);
        switch (s.type) {
            case SHAPE_LINE:
                printf("Line: Point1(%d, %d), Point2(%d, %d)\n", 
                       s.params.line.r1, s.params.line.c1, 
                       s.params.line.r2, s.params.line.c2);
                break;
            case SHAPE_RECTANGLE:
                printf("Rectangle: Top-Left(%d, %d), Width: %d, Height: %d\n", 
                       s.params.rect.row, s.params.rect.col, 
                       s.params.rect.width, s.params.rect.height);
                break;
            case SHAPE_CIRCLE:
                printf("Circle: Center(%d, %d), Radius: %d\n", 
                       s.params.circle.row, s.params.circle.col, 
                       s.params.circle.radius);
                break;
            case SHAPE_TRIANGLE:
                printf("Triangle: Pt1(%d, %d), Pt2(%d, %d), Pt3(%d, %d)\n", 
                       s.params.triangle.r1, s.params.triangle.c1, 
                       s.params.triangle.r2, s.params.triangle.c2, 
                       s.params.triangle.r3, s.params.triangle.c3);
                break;
        }
    }
}

void addShape()
{
    if (shape_count >= MAX_SHAPES) {
        printf("Error: Maximum shape limit reached (%d).\n", MAX_SHAPES);
        return;
    }

    printf("\nChoose shape type to add:\n");
    printf("1. Line\n");
    printf("2. Rectangle\n");
    printf("3. Circle\n");
    printf("4. Triangle\n");
    int type_choice = getIntegerInput("Enter choice: ");

    if (type_choice < 1 || type_choice > 4) {
        printf("Invalid shape type.\n");
        return;
    }

    Shape s;
    s.id = next_id++;
    s.type = (ShapeType)type_choice;

    switch (s.type) {
        case SHAPE_LINE:
            s.params.line.r1 = getIntegerInput("Enter start row: ");
            s.params.line.c1 = getIntegerInput("Enter start column: ");
            s.params.line.r2 = getIntegerInput("Enter end row: ");
            s.params.line.c2 = getIntegerInput("Enter end column: ");
            break;
        case SHAPE_RECTANGLE:
            s.params.rect.row = getIntegerInput("Enter top-left row: ");
            s.params.rect.col = getIntegerInput("Enter top-left column: ");
            s.params.rect.width = getIntegerInput("Enter width: ");
            s.params.rect.height = getIntegerInput("Enter height: ");
            break;
        case SHAPE_CIRCLE:
            s.params.circle.row = getIntegerInput("Enter center row: ");
            s.params.circle.col = getIntegerInput("Enter center column: ");
            s.params.circle.radius = getIntegerInput("Enter radius: ");
            break;
        case SHAPE_TRIANGLE:
            s.params.triangle.r1 = getIntegerInput("Enter point 1 row: ");
            s.params.triangle.c1 = getIntegerInput("Enter point 1 column: ");
            s.params.triangle.r2 = getIntegerInput("Enter point 2 row: ");
            s.params.triangle.c2 = getIntegerInput("Enter point 2 column: ");
            s.params.triangle.r3 = getIntegerInput("Enter point 3 row: ");
            s.params.triangle.c3 = getIntegerInput("Enter point 3 column: ");
            break;
    }

    shapes[shape_count++] = s;
    redrawCanvas();
    printf("Shape added successfully with ID: %d!\n", s.id);
}

void deleteShape()
{
    if (shape_count == 0) {
        printf("No shapes to delete.\n");
        return;
    }
    
    listShapes();
    int target_id = getIntegerInput("Enter the ID of the shape to delete: ");

    int index = -1;
    for (int i = 0; i < shape_count; i++) {
        if (shapes[i].id == target_id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Error: Shape with ID %d not found.\n", target_id);
        return;
    }

    // Shift remaining elements
    for (int i = index; i < shape_count - 1; i++) {
        shapes[i] = shapes[i + 1];
    }
    shape_count--;

    redrawCanvas();
    printf("Shape with ID %d deleted successfully.\n", target_id);
}

void modifyShape()
{
    if (shape_count == 0) {
        printf("No shapes to modify.\n");
        return;
    }

    listShapes();
    int target_id = getIntegerInput("Enter the ID of the shape to modify: ");

    int index = -1;
    for (int i = 0; i < shape_count; i++) {
        if (shapes[i].id == target_id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Error: Shape with ID %d not found.\n", target_id);
        return;
    }

    Shape *s = &shapes[index];
    printf("Modifying Shape ID: %d (Type: ", s->id);
    switch (s->type) {
        case SHAPE_LINE: printf("Line)\n"); break;
        case SHAPE_RECTANGLE: printf("Rectangle)\n"); break;
        case SHAPE_CIRCLE: printf("Circle)\n"); break;
        case SHAPE_TRIANGLE: printf("Triangle)\n"); break;
    }

    switch (s->type) {
        case SHAPE_LINE:
            s->params.line.r1 = getIntegerInput("Enter new start row: ");
            s->params.line.c1 = getIntegerInput("Enter new start column: ");
            s->params.line.r2 = getIntegerInput("Enter new end row: ");
            s->params.line.c2 = getIntegerInput("Enter new end column: ");
            break;
        case SHAPE_RECTANGLE:
            s->params.rect.row = getIntegerInput("Enter new top-left row: ");
            s->params.rect.col = getIntegerInput("Enter new top-left column: ");
            s->params.rect.width = getIntegerInput("Enter new width: ");
            s->params.rect.height = getIntegerInput("Enter new height: ");
            break;
        case SHAPE_CIRCLE:
            s->params.circle.row = getIntegerInput("Enter new center row: ");
            s->params.circle.col = getIntegerInput("Enter new center column: ");
            s->params.circle.radius = getIntegerInput("Enter new radius: ");
            break;
        case SHAPE_TRIANGLE:
            s->params.triangle.r1 = getIntegerInput("Enter new point 1 row: ");
            s->params.triangle.c1 = getIntegerInput("Enter new point 1 column: ");
            s->params.triangle.r2 = getIntegerInput("Enter new point 2 row: ");
            s->params.triangle.c2 = getIntegerInput("Enter new point 2 column: ");
            s->params.triangle.r3 = getIntegerInput("Enter new point 3 row: ");
            s->params.triangle.c3 = getIntegerInput("Enter new point 3 column: ");
            break;
    }

    redrawCanvas();
    printf("Shape with ID %d modified successfully.\n", s->id);
}

int main()
{
    int choice;

    initializeCanvas();

    do
    {
        printf("\n===== 2D GRAPHICS EDITOR =====\n");
        printf("1. Add Object\n");
        printf("2. Delete Object\n");
        printf("3. Modify Object\n");
        printf("4. Display Picture (Canvas)\n");
        printf("5. List Objects\n");
        printf("6. Clear Canvas (Delete All)\n");
        printf("7. Exit\n");
        
        choice = getIntegerInput("Enter choice: ");

        switch(choice)
        {
            case 1:
                addShape();
                break;

            case 2:
                deleteShape();
                break;

            case 3:
                modifyShape();
                break;

            case 4:
                displayCanvas();
                break;

            case 5:
                listShapes();
                break;

            case 6:
                shape_count = 0;
                redrawCanvas();
                printf("Canvas Cleared (All objects deleted)!\n");
                break;

            case 7:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid Choice!\n");
        }

    } while(choice != 7);

    return 0;
}
