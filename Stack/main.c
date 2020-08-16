#include <stdio.h>

int MAX_SIZE = 8;
int stack[8];
int top = -1;

int isEmpty() {
  if(top == -1) {
    return 1;
  } else {
    return 0;
  }
}

int isFull() {
  if(top == MAX_SIZE) {
    return 1;
  } else {
    return 0;
  }
}

int peek() {
  return stack[top]
}

int pop() {
  int data;

  if(!isEmpty()) {
    data = stack[top];
    top = top - 1;

    return data;
  } else {
    printf("Stack is empty");
  }
}

int push(int data) {
  if(!isFull) {
    top = top + 1;
    stack[top] = data;
  } else {
    printf("Stack is full");
  }
}

int main() {
  // Push items to the stack
  push(29);
  push(6);
  push(98);

  printf("Current top of the stack: %d\n", peek());
  printf("Elements: \n");

  while(!isEmpty()) {
    int data = pop();
    
    printf("%d\n", data);
  }

  printf("The stack is %s\n", isFull() ? "full" : "not full yet");
  printf("The stack is %s\n", isEmpty() ? "empty" : "not empty yet");

  return 0;
}