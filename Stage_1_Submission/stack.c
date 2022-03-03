#include "stack.h"
#include "parser.h"

Stack* createStack() {
    Stack * stack = (Stack *) malloc(sizeof(Stack));
    stack -> size = 0;
    stack -> top = NULL;
    stack -> bottom = NULL;
    return stack;
}

StackNode* pop(Stack * stack) {
    if (stack -> size == 0)
      return NULL;
  
    StackNode* temp = stack -> top;
    stack -> top = stack -> top -> next;
    stack -> size = stack -> size - 1;
    return temp;
}

StackNode* top(Stack * stack) {
    if(stack==NULL || stack -> size==0)
      return NULL;
  
    return stack -> top;
}

void push(Stack* stack, stackData* stack_data) {
    // StackNode* new_node = createStackNode(stack_data);
    StackNode * new_node = (StackNode *) malloc(sizeof(StackNode));
    new_node -> next = NULL;
    new_node -> stack_data = stack_data;
    switch (stack -> size) {
        case 0:
            stack -> top = new_node;
            stack -> bottom = new_node;
            stack -> size= stack -> size + 1;
            break;
        default:
            new_node -> next = stack -> top;
            stack -> top = new_node;
            stack -> size = stack -> size + 1;
            break;
    }
}

void pushAllChildrenOnStack(Stack * stack, stackData * stack_data) {
    if (stack_data==NULL)
      return;
    pushAllChildrenOnStack(stack, stack_data->nextSibling);
    push(stack,stack_data);
}

void printStack(Stack * stack) {
    StackNode * temp = stack->top;
    while(temp!=NULL)
    {
        printf("%d\n",temp->stack_data->isTerminal);
        temp = temp->next;
    }
}