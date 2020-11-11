#include "Node.h"
#include "Guards.h"

Node* ErrorNode_new(const char *msg)
{
    Node *node = malloc(sizeof(Node));
    OOM_GUARD(node, __FILE__, __LINE__);
    node->type = ERROR_NODE;
    node->data.error = msg;
    return node;
}

Node* CommandNode_new(StrVec words)
{
    Node *node = malloc(sizeof(Node));
    OOM_GUARD(node, __FILE__, __LINE__);
    node->type = COMMAND_NODE;
    node->data.command = words;
    return node;
}

Node* PipeNode_new(Node *left, Node *right)
{
    Node *node = malloc(sizeof(Node));
    OOM_GUARD(node, __FILE__, __LINE__);
    node->type = PIPE_NODE;
    node->data.pipe.left = left;
    node->data.pipe.right = right;
    return node;
}

void* Node_drop(Node *self)
{
    if (self->type == PIPE_NODE) {
        StrVec *left = (&self->data.pipe.left->data.command);
        StrVec_drop(left);
       StrVec *right = (&self->data.pipe.right->data.command); 
        StrVec_drop(right);
    }
    StrVec_drop(&self->data.command);
    return NULL;
}
