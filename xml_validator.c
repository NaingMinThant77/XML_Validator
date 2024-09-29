#include <stdio.h>
#include <string.h>

// Define the maximum tag length and stack size
#define MAX_TAG_LENGTH 100
#define MAX_STACK_SIZE 100

// Define a stack structure to store tags
typedef struct
{
    char tags[MAX_STACK_SIZE][MAX_TAG_LENGTH];
    int top;
} Stack;

// Function to initialize the stack
void initStack(Stack *stack)
{
    stack->top = -1;
}

// Function to push a tag onto the stack
void push(Stack *stack, char *tag)
{
    if (stack->top < MAX_STACK_SIZE - 1)
    {
        strcpy(stack->tags[++stack->top], tag);
    }
}

// Function to pop a tag from the stack
char *pop(Stack *stack) {
    if (stack->top >= 0) {
        return stack->tags[stack->top--]; // Retrieve and then decrement the top
    }
    return NULL;
}
    //    return (stack->top >= 0) ? stack->tags[stack->top] : NULL;


// Function to check if the stack is empty
int isEmpty(Stack *stack)
{
    return stack->top == -1;
}

// Function to check if two tags match
int tagsMatch(const char *openingTag, const char *closingTag)
{
    return strcmp(openingTag, closingTag) == 0;
}

// Function to skip comments
int skipComment(FILE *file)
{
    int c;
    while ((c = fgetc(file)) != EOF)
    { // End of File // returns the ASCII code of the character
        if (c == '-' && (c = fgetc(file)) == '-' && (c = fgetc(file)) == '>')
        {
            return 1; // Comment ended
        }
    }
    return 0; // Error or unexpected end of file
}

// Validate the XML structure
int validateXML(FILE *file)
{
    Stack stack;
    initStack(&stack);
    char tag[MAX_TAG_LENGTH];
    int c, inTag = 0, isClosingTag = 0, tagLength = 0;

    while ((c = fgetc(file)) != EOF)
    {
        if (c == '<')
        {
            inTag = 1;
            isClosingTag = 0;
            tagLength = 0;

            // Check for comments
            if ((c = fgetc(file)) == '!')
            {
                if (skipComment(file))
                {
                    continue; // Skip over the comment
                }
            }
            else
            {
                ungetc(c, file);
            }
        }
        else if (c == '>')
        {
            tag[tagLength] = '\0'; // Null-terminates
            inTag = 0;

            // Skip self-closing tags
            if (tag[tagLength - 1] == '/')
                continue;

            if (isClosingTag)
            {
                if (isEmpty(&stack) || !tagsMatch(pop(&stack), tag)) {
                    return 0; // XML is invalid
                }
            }
            else
            {
                push(&stack, tag);
            }
        }
        else if (inTag)
        {
            if (c == '/')
            {
                isClosingTag = 1;
            }
            else
            {
                tag[tagLength++] = c;
            }
        }
    }

    return isEmpty(&stack);
}

int main()
{
    FILE *file = fopen("Assignment/xml_validator/invalid.xml", "r"); // "r" stands for read only mode
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return 1; // return 0; = Success. return 1; (or any non-zero value) = Error or abnormal termination.
    }

    if (validateXML(file))
    {
        printf("XML is valid.\n");
    }
    else
    {
        printf("XML is invalid.\n");
    }

    fclose(file);
    return 0;
}
