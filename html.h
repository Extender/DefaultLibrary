#ifndef HTML_H
#define HTML_H

#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include "text.h"
#include "io.h"

#define node_id_t uint32_t
#define node_type_t uint8_t
#define node_invalidId UINT32_MAX
#define node_invalidIndex node_invalidId

#define NODE_TYPE_TEXT 1
#define NODE_TYPE_ELEMENT 2
#define NODE_TYPE_ATTRIBUTE 4
#define NODE_TYPE_COMMENT 8
#define NODE_TYPE_PLAINTEXT 16 // Includes script/css/plaintext nodes; NODE_TYPE_ELEMENT must be set.
#define NODE_TYPE_CDATA 32
#define NODE_TYPE_DOCTYPE 64
#define NODE_TYPE_ROOT 128
#define NODE_TYPE_DEFAULT NODE_TYPE_ELEMENT
#define DEFAULT_NODE_NAME ""
#define DEFAULT_NODE_CHILD_BUFFER_SIZE 8
#define DEFAULT_NODE_BUFFER_SIZE 8
#define DEFAULT_NODE_PARSER_BUFFER_SIZE 256
#define DEFAULT_NODE_PARSER_ATTR_NAME_BUFFER_SIZE 16
#define DEFAULT_NODE_PARSER_ATTR_VALUE_BUFFER_SIZE 32
#define DEFAULT_NODE_PARSER_TAG_NAME_BUFFER_SIZE 8
#define DEFAULT_NODE_PARSER_TEXT_CONTENT_BUFFER_SIZE 128
#define DEFAULT_HTML_SERIALIZER_BUFFER_SIZE 256

#define pause() system("pause")

namespace html
{
    class Node
    {
        void initNode(node_id_t _id,node_type_t _type,char *_name,fs_t _valueLength,char *_value);

    public:
        node_id_t id;
        node_type_t type;
        char *name;
        fs_t valueBufferSize;
        fs_t valueLength;
        char *value;
        node_id_t parent;
        node_id_t childBufferSize;
        node_id_t childCount;
        node_id_t *children;

        Node();
        ~Node();
        Node(node_id_t _id);
        Node(node_id_t _id,node_type_t _type);
        Node(node_id_t _id,char *_name);
        Node(node_id_t _id,node_type_t _type,char *_name);
        Node(node_id_t _id,char *_name,fs_t _valueLength,char *_value);
        Node(node_id_t _id,node_type_t _type,fs_t _valueLength,char *_value);
        Node(node_id_t _id,node_type_t _type,char *_name,fs_t _valueLength,char *_value);
    };

    class NodeCollection
    {
    public:
        static node_id_t indexOf(Node **nodes,node_id_t nodeCount,Node *node);
        static Node *getNodeById(Node **nodes,node_id_t nodeCount,node_id_t id);
        static Node *getNodeByName(Node **nodes,node_id_t nodeCount,char *name);
        static Node *getNodeByCaseSensitiveName(Node **nodes,node_id_t nodeCount,char *name);
        static Node **getNodesByType(Node **nodes,node_id_t nodeCount,node_type_t typeFlags /*flags*/,node_id_t &count);
        static Node **getNodesByIds(Node **nodes,node_id_t nodeCount,node_id_t *ids,node_id_t idCount);
        static node_id_t childIndex(Node *node,node_id_t childId);
        static node_id_t idIndex(node_id_t *ids,node_id_t count,node_id_t id);
        static void addChild(Node *node,node_id_t childId);
        static void removeChild(Node *node,node_id_t childId);
        static void insertChild(Node *node,node_id_t childId,node_id_t pos);
        static Node **getChildren(Node **nodes,node_id_t nodeCount,Node *node);
        static void addNode(Node **&nodes,node_id_t &nodeCount,node_id_t &bufferSize,Node *node);
        static void removeNode(Node **&nodes,node_id_t &nodeCount,node_id_t &bufferSize,Node *node);
    };

    class parser
    {
    public:
        static Node **parse(char *source,node_id_t &nodeCount,node_id_t &bufferSize,Node *&rootNode);
        static char *serialize(Node **nodes,node_id_t size,Node *rootNode);
    };
}

#endif // HTML_H
