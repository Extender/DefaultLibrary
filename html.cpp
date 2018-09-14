#include "html.h"

node_id_t html::NodeCollection::indexOf(html::Node **nodes, node_id_t nodeCount, html::Node *node)
{
    for(node_id_t i=0;i<nodeCount;i++)
    {
        if(nodes[i]==node)
            return i;
    }
    return node_invalidIndex;
}

html::Node *html::NodeCollection::getNodeById(Node **nodes, node_id_t nodeCount, node_id_t id)
{
    for(node_id_t i=0;i<nodeCount;i++)
    {
        Node *thisNode=nodes[i];
        if(thisNode->id==id)
            return thisNode;
    }
    return 0;
}

html::Node *html::NodeCollection::getNodeByName(Node **nodes, node_id_t nodeCount, char *name)
{
    for(node_id_t i=0;i<nodeCount;i++)
    {
        Node *thisNode=nodes[i];
        if(stricmp(thisNode->name,name)==0)
            return thisNode;
    }
    return 0;
}

html::Node *html::NodeCollection::getNodeByCaseSensitiveName(Node **nodes, node_id_t nodeCount, char *name)
{
    for(node_id_t i=0;i<nodeCount;i++)
    {
        Node *thisNode=nodes[i];
        if(strcmp(thisNode->name,name)==0)
            return thisNode;
    }
    return 0;
}

html::Node **html::NodeCollection::getNodesByType(Node **nodes, node_id_t nodeCount, node_type_t typeFlags /*flags*/, node_id_t &count)
{
    fs_t bufferSize=DEFAULT_NODE_BUFFER_SIZE;
    count=0;
    Node **buffer=(Node**)malloc(bufferSize*sizeof(Node*));
    for(node_id_t i=0;i<nodeCount;i++)
    {
        Node *thisNode=nodes[i];
        if((thisNode->type&typeFlags)!=0)
        {
            if(count==bufferSize)
            {
                bufferSize*=2;
                buffer=(Node**)realloc(buffer,bufferSize*sizeof(Node*));
            }
            buffer[count++]=thisNode;
        }
    }
    return buffer;
}

html::Node **html::NodeCollection::getNodesByIds(Node **nodes, node_id_t nodeCount, node_id_t *ids, node_id_t idCount)
{
    fs_t bufferSize=DEFAULT_NODE_BUFFER_SIZE;
    node_id_t count=0;
    Node **buffer=(Node**)malloc(bufferSize*sizeof(Node*));
    for(node_id_t i=0;i<nodeCount;i++)
    {
        Node *thisNode=nodes[i];
        if(idIndex(ids,idCount,thisNode->id)!=node_invalidIndex)
        {
            if(count==bufferSize)
            {
                bufferSize*=2;
                buffer=(Node**)realloc(buffer,bufferSize*sizeof(Node*));
            }
            buffer[count++]=thisNode;
        }
    }
    return buffer;
}

node_id_t html::NodeCollection::childIndex(html::Node *node, node_id_t childId)
{
    for(node_id_t i=0;i<node->childCount;i++)
    {
        if(node->children[i]==childId)
            return i;
    }
    return node_invalidIndex;
}

node_id_t html::NodeCollection::idIndex(node_id_t *ids, node_id_t count, node_id_t id)
{
    for(node_id_t i=0;i<count;i++)
    {
        if(ids[i]==id)
            return i;
    }
    return node_invalidIndex;
}

void html::NodeCollection::addChild(html::Node *node, node_id_t childId)
{
    if(node->childBufferSize<=node->childCount)
    {
        node->childBufferSize*=2;
        node->children=(node_id_t*)realloc(node->children,node->childBufferSize*sizeof(node_id_t));
    }
    node->children[node->childCount++]=childId;
}

void html::NodeCollection::removeChild(html::Node *node, node_id_t childId)
{
    node_id_t index=childIndex(node,childId);
    if(index==node->childCount-1)
    {
        node->childCount--;
        return;
    }
    node_id_t *newChildBuffer=(node_id_t*)malloc(node->childBufferSize*sizeof(node_id_t));
    memcpy(newChildBuffer,node->children,index*sizeof(node_id_t)); // Entries before this entry
    memcpy(newChildBuffer+index,node->children+index+1,(node->childCount-index-1)*sizeof(node_id_t)); // Entries after this entry
    free(node->children);
    node->children=newChildBuffer;
    node->childCount--;
}

void html::NodeCollection::insertChild(html::Node *node, node_id_t childId, node_id_t pos)
{
    if(node->childBufferSize>=node->childCount)
    {
        node->childBufferSize*=2;
        node->children=(node_id_t*)realloc(node->children,node->childBufferSize*sizeof(node_id_t));
    }
    node_id_t *newChildBuffer=(node_id_t*)malloc(node->childBufferSize*sizeof(node_id_t));
    memcpy(newChildBuffer,node->children,pos*sizeof(node_id_t));
    newChildBuffer[pos]=childId;
    if(pos!=node->childCount)
        memcpy(newChildBuffer+pos+1,node->children+pos,pos);
    free(node->children);
    node->children=newChildBuffer;
    node->childCount++;
}

html::Node **html::NodeCollection::getChildren(Node **nodes, node_id_t nodeCount, html::Node *node)
{
    return getNodesByIds(nodes,nodeCount,node->children,node->childCount);
}

void html::NodeCollection::addNode(html::Node **&nodes, node_id_t &nodeCount, node_id_t &bufferSize, html::Node *node)
{
    if(nodeCount>=bufferSize)
    {
        bufferSize*=2;
        nodes=(Node**)realloc(nodes,bufferSize*sizeof(Node*));
    }
    nodes[nodeCount++]=node;
}

void html::NodeCollection::removeNode(html::Node **&nodes, node_id_t &nodeCount, node_id_t &bufferSize, html::Node *node)
{
    // Does not delete the node.
    node_id_t index=indexOf(nodes,nodeCount,node);
    if(index==nodeCount-1)
    {
        nodeCount--;
        return;
    }
    Node **newBuffer=(Node**)malloc(bufferSize*sizeof(Node*));
    memcpy(newBuffer,nodes,index*sizeof(Node*)); // Entries before this entry
    memcpy(newBuffer+index,nodes+index+1,(nodeCount-index-1)*sizeof(Node*)); // Entries after this entry
    free(nodes);
    nodes=newBuffer;
    nodeCount--;
}

void html::Node::initNode(node_id_t _id,node_type_t _type,char *_name,fs_t _valueLength,char *_value)
{
    id=_id;
    type=_type;
    name=_name;
    valueLength=_valueLength;
    valueBufferSize=valueLength;
    value=_value;
    parent=node_invalidId;
    childBufferSize=DEFAULT_NODE_CHILD_BUFFER_SIZE;
    childCount=0;
    children=(node_id_t*)malloc(childBufferSize*sizeof(node_id_t));
}

html::Node::Node()
{
    initNode(node_invalidId,NODE_TYPE_DEFAULT,text::duplicateString(DEFAULT_NODE_NAME),0,strdup(""));
}

html::Node::~Node()
{
    free(name);
    free(value);
    free(children);
}

html::Node::Node(node_id_t _id)
{
    initNode(_id,NODE_TYPE_DEFAULT,text::duplicateString(DEFAULT_NODE_NAME),0,text::duplicateString(""));
}

html::Node::Node(node_id_t _id, node_type_t _type)
{
    initNode(_id,_type,text::duplicateString(DEFAULT_NODE_NAME),0,text::duplicateString(""));
}

html::Node::Node(node_id_t _id, char *_name)
{
    initNode(_id,NODE_TYPE_DEFAULT,text::duplicateString(_name),0,text::duplicateString(""));
}

html::Node::Node(node_id_t _id, node_type_t _type, char *_name)
{
    initNode(_id,_type,text::duplicateString(_name),0,text::duplicateString(""));
}

html::Node::Node(node_id_t _id, char *_name, fs_t _valueLength, char *_value)
{
    initNode(_id,NODE_TYPE_DEFAULT,text::duplicateString(_name),_valueLength,text::duplicateString(_value));
}

html::Node::Node(node_id_t _id, node_type_t _type, fs_t _valueLength, char *_value)
{
    initNode(_id,_type,text::duplicateString(DEFAULT_NODE_NAME),_valueLength,text::duplicateString(_value));
}

html::Node::Node(node_id_t _id, node_type_t _type, char *_name, fs_t _valueLength, char *_value)
{
    initNode(_id,_type,text::duplicateString(_name),_valueLength,text::duplicateString(_value));
}

const char *html::Node::nodeTypeToString(node_type_t type)
{
    // Order of checking plays an important role!

    if((type & NODE_TYPE_TEXT) == NODE_TYPE_TEXT)
        return "Text";
    if((type & NODE_TYPE_ATTRIBUTE) == NODE_TYPE_ATTRIBUTE)
        return "Attribute";
    if((type & NODE_TYPE_COMMENT) == NODE_TYPE_COMMENT)
        return "Comment";
    if((type & NODE_TYPE_PLAINTEXT) == NODE_TYPE_PLAINTEXT)
        return "Plaintext element";
    if((type & NODE_TYPE_CDATA) == NODE_TYPE_CDATA)
        return "CDATA";
    if((type & NODE_TYPE_DOCTYPE) == NODE_TYPE_DOCTYPE)
        return "DOCTYPE";
    if((type & NODE_TYPE_ROOT) == NODE_TYPE_ROOT)
        return "Root";
    if((type & NODE_TYPE_ELEMENT) == NODE_TYPE_ELEMENT)
        return "Element";
}

html::Node::Node(uint32_t _id, const char *_name)
{
    initNode(_id,NODE_TYPE_DEFAULT,text::duplicateString(_name),0,text::duplicateString(""));
}

html::Node::Node(uint32_t _id, uint8_t _type, const char *_name)
{
    initNode(_id,_type,text::duplicateString(_name),0,text::duplicateString(""));
}

html::Node::Node(uint32_t _id, const char *_name, const char *_value)
{
    initNode(_id,NODE_TYPE_DEFAULT,text::duplicateString(_name),strlen(_value),text::duplicateString(_value));
}

html::Node::Node(uint32_t _id, uint8_t _type, const char *_name, const char *_value)
{
    initNode(_id,_type,text::duplicateString(_name),strlen(_value),text::duplicateString(_value));
}

html::Node **html::parser::parse(char *source, node_id_t &nodeCount, node_id_t &bufferSize, Node *&rootNode)
{
    // Variables

    text_t sourceLength=strlen(source);
    node_id_t nextNodeId=1; // Start with 1 just to be safe.
    char thisChar;
    char prevChar=0;
    char prevNonWhitespaceChar=0;
    bool isWhitespace;
    bool isString=false;
    bool isTag=false;
    bool isAttrName=false;
    bool isAttrValue=false;
    char stringChar;
    bool isEndTag=false;
    bool isSelfClosingTag=false;
    bool isTextNode=false;
    bool isComment=false;
    bool isCDATA=false;
    bool isPlaintext=false;
    bool isHtmlTag; // This actually does get used in HTML_PARSER_FINALIZE_TAG.
    bool attrNameHadWhitespace=false;
    bool tagHadAttrWithStringValue=false; // We need a string value, it could be a mathematical equation or something similar.
    bool isDoctypeDeclaration=false;
    char *plaintextEnd=0;
    text_t lastOpeningTagPosition=pos_notFound;
    text_t lastOpeningTagPositionInPlaintext=pos_notFound;
    text_t plaintextEndLen;

    text_t nonWhitespaceStringLength=0;
    char *nonWhitespaceString=(char*)malloc(sourceLength);

    // Buffers

    bufferSize=DEFAULT_NODE_PARSER_BUFFER_SIZE;
    nodeCount=0;
    Node **nodes=(Node**)malloc(bufferSize*sizeof(Node*));

    node_id_t openNodeBufferSize=DEFAULT_NODE_PARSER_BUFFER_SIZE;
    node_id_t openNodeCollectionLength=0;
    Node **openNodes=(Node**)malloc(openNodeBufferSize*sizeof(Node*));

    fs_t attrNameBufferSize=DEFAULT_NODE_PARSER_ATTR_NAME_BUFFER_SIZE;
    fs_t attrNameLength=0;
    char *attrName=(char*)malloc(attrNameBufferSize);

    fs_t attrValueBufferSize=DEFAULT_NODE_PARSER_ATTR_VALUE_BUFFER_SIZE;
    fs_t attrValueLength=0;
    char *attrValue=(char*)malloc(attrValueBufferSize);

    fs_t tagNameBufferSize=DEFAULT_NODE_PARSER_TAG_NAME_BUFFER_SIZE;
    fs_t tagNameLength=0;
    char *tagName=(char*)malloc(tagNameBufferSize);

    // Define macros

    #define HTML_PARSER_NEXT_ITERATION \
    \
    "Example: \\";"(count the closing quotation mark, too)";\
    if(!(prevChar=='\\'&&thisChar=='\\'))\
        prevChar=thisChar;\
    else\
        prevChar=0;\
    if(!isWhitespace)\
    {\
        prevNonWhitespaceChar=thisChar;\
        nonWhitespaceString[nonWhitespaceStringLength++]=thisChar;\
    }\
    continue;

    #define HTML_PARSER_NEW_NODE \
    \
    lastOpeningTagPosition=i;\
    if(isTextNode)\
    {\
        Node *textNode=nodes[nodeCount-1];\
        io::terminateBuffer(textNode->value,textNode->valueLength,textNode->valueBufferSize);\
        isTextNode=false;\
    }\
    isTag=true;\
    node_id_t id=nextNodeId++;\
    Node *n=new Node(id,NODE_TYPE_ELEMENT);\
    n->parent=openNodes[openNodeCollectionLength-1]->id;\
    NodeCollection::addChild(openNodes[openNodeCollectionLength-1],id);\
    NodeCollection::addNode(openNodes,openNodeCollectionLength,openNodeBufferSize,n);

    #define HTML_PARSER_ADD_ATTRIBUTE \
    \
    node_id_t id=nextNodeId++;\
    char *firstChars=text::firstChars(attrValue,attrValueLength);\
    char *unesc=stringChar=='"'?text::unescapeDoubleQuotationMarks(firstChars):text::unescapeSingleQuotationMarks(firstChars);\
    free(firstChars);\
    Node *n=new Node(id,NODE_TYPE_ATTRIBUTE,text::duplicateString(attrName),strlen(unesc),unesc);\
    n->parent=openNodes[openNodeCollectionLength-1]->id;\
    NodeCollection::addChild(openNodes[openNodeCollectionLength-1],id);\
    NodeCollection::addNode(nodes,nodeCount,bufferSize,n);\
    free(attrName);\
    attrNameBufferSize=DEFAULT_NODE_PARSER_ATTR_NAME_BUFFER_SIZE;\
    attrNameLength=0;\
    attrName=(char*)malloc(attrNameBufferSize);\
    attrValueBufferSize=DEFAULT_NODE_PARSER_ATTR_VALUE_BUFFER_SIZE;\
    attrValueLength=0;\
    free(attrValue);\
    attrValue=(char*)malloc(attrValueBufferSize);

    #define HTML_PARSER_CLEAR_BUFFERS \
    "This does not remove the node from openNodes (needed that way)!";\
    isTag=false;\
    isEndTag=false;\
    isSelfClosingTag=false;\
    tagHadAttrWithStringValue=false;\
    tagNameBufferSize=DEFAULT_NODE_PARSER_TAG_NAME_BUFFER_SIZE;\
    tagNameLength=0;\
    free(tagName);\
    tagName=(char*)malloc(tagNameBufferSize);

    #define HTML_PARSER_FINALIZE_TAG \
    \
    \
    if(isEndTag)\
    {\
        "Search for a matching opening tag.";\
        "WARNING: TEST THIS";\
        Node *thisNode=openNodes[openNodeCollectionLength-1];\
        isHtmlTag=stricmp(tagName,"html")==0;\
        node_id_t toBeRemoved=0;\
        "Remove this end tag node from its parent's children";\
        "There's always the root node, so this will work.";\
        NodeCollection::removeChild(openNodes[openNodeCollectionLength-2],thisNode->id);\
        thisNode->parent=0;\
    openNodeCollectionLength--;\
        "Because it is unsigned, j is always 1 value higher than the actual j we need to prevent it from overflowing.";\
        for(node_id_t j=openNodeCollectionLength/*-1*/;j>0;j--)\
        {\
            toBeRemoved++;\
            Node *aNode=openNodes[j-1];\
    "Assume the type is NODE_TYPE_ELEMENT";\
    if((stricmp(aNode->name,tagName)==0||(isHtmlTag&&stricmp(aNode->name,"!DOCTYPE")==0)))\
                break;\
        }\
        if(toBeRemoved<openNodeCollectionLength)\
        {\
            "Else: stray end tag.";\
            openNodeCollectionLength-=toBeRemoved;\
        }\
    }\
    else\
    {\
        Node *thisNode=openNodes[openNodeCollectionLength-1];\
        NodeCollection::addNode(nodes,nodeCount,bufferSize,thisNode);\
        "Check for self-closing tags, singleton tags, plaintext tags";\
        if(isSelfClosingTag)\
            NodeCollection::removeNode(openNodes,openNodeCollectionLength,openNodeBufferSize,thisNode);\
        else\
        {\
            "Modify in \"serialize\", too.";\
            if(stricmp(tagName,"img")==0||\
               stricmp(tagName,"br")==0||\
               stricmp(tagName,"input")==0||\
               stricmp(tagName,"link")==0||\
               stricmp(tagName,"meta")==0||\
               stricmp(tagName,"hr")==0||\
               stricmp(tagName,"embed")==0||\
               stricmp(tagName,"param")==0||\
               stricmp(tagName,"area")==0||\
               stricmp(tagName,"base")==0||\
               stricmp(tagName,"source")==0||\
               stricmp(tagName,"col")==0||\
               stricmp(tagName,"command")==0\
              )\
                NodeCollection::removeNode(openNodes,openNodeCollectionLength,openNodeBufferSize,thisNode);\
            else if(stricmp(tagName,"style")==0||\
                    stricmp(tagName,"script")==0||\
                    stricmp(tagName,"plaintext")==0)\
                    {\
                        isPlaintext=true;\
                        "Do not set this to NODE_TYPE_PLAINTEXT.";\
                        thisNode->type|=NODE_TYPE_PLAINTEXT;\
                        free(plaintextEnd);\
                        plaintextEnd=text::concat("</",tagName,">");\
                        plaintextEndLen=strlen(plaintextEnd);\
                        "Allocate value buffer:";\
                        thisNode->valueBufferSize=DEFAULT_NODE_PARSER_TEXT_CONTENT_BUFFER_SIZE;\
                        free(thisNode->value);\
                        thisNode->valueLength=0;\
                        thisNode->value=(char*)malloc(thisNode->valueBufferSize);\
                        "Do not remove this node from openNodes right after the opening tag ends, there will be attribute nodes!";\
                    }\
            ;\
        }\
    }\
    HTML_PARSER_CLEAR_BUFFERS;

    #define HTML_PARSER_CHECK_IF_TAG_IS_VALID \
    \
    if(!tagHadAttrWithStringValue)\
    {\
        "Turn this into a text node.";\
        Node *thisNode=openNodes[openNodeCollectionLength-1];\
        Node *parentNode=NodeCollection::getNodeById(openNodes,openNodeBufferSize,thisNode->parent);\
        NodeCollection::removeChild(parentNode,thisNode->id);\
        node_id_t newId=nextNodeId++;\
        "WARNING: TEST THIS!";\
        text_t valueLength=i-lastOpeningTagPosition;\
        char *value=text::substr(source,lastOpeningTagPosition,valueLength);\
        Node *textNode=new Node(newId,NODE_TYPE_TEXT,text::duplicateString(""),valueLength,value);\
        textNode->parent=thisNode->parent;\
        "Delete this node's child nodes";\
        for(node_id_t j=0;j<thisNode->childCount;j++)\
        {\
            Node *nodeToRemove=NodeCollection::getNodeById(nodes,nodeCount,thisNode->children[j]);\
            NodeCollection::removeNode(nodes,nodeCount,bufferSize,nodeToRemove);\
            delete nodeToRemove;\
        }\
        NodeCollection::addChild(parentNode,newId);\
        NodeCollection::addNode(nodes,nodeCount,bufferSize,textNode);\
        NodeCollection::removeNode(openNodes,openNodeCollectionLength,openNodeBufferSize,thisNode);\
        HTML_PARSER_CLEAR_BUFFERS;\
        "Needed.";\
        isAttrName=false;\
        isAttrValue=false;\
        HTML_PARSER_NEW_NODE;\
    }

    // End define macros

    // Add root node to openNodes and nodes

    rootNode=new Node(nextNodeId++,NODE_TYPE_ROOT);
    NodeCollection::addNode(openNodes,openNodeCollectionLength,openNodeBufferSize,rootNode);
    NodeCollection::addNode(nodes,nodeCount,bufferSize,rootNode);

    // Parsing loop (char-to-char)

    for(text_t i=0;i<sourceLength;i++)
    {
        thisChar=source[i];
        isWhitespace=thisChar==' '||thisChar=='\n'||thisChar=='\r'||thisChar=='\t';
        if(isTag) // Inside a <> tag.
        {
            if(isAttrName)
            {
                if(isWhitespace)
                { // {} needed here.
                    if(attrNameLength>0) // Catch boolean attributes.
                        attrNameHadWhitespace=true;
                    HTML_PARSER_NEXT_ITERATION;
                }
                if(thisChar=='>')
                {
                    isAttrName=false;
                    if(attrNameLength>0)
                    {
                        // This is a boolean attribute at the end of the tag.
                        io::terminateBuffer(attrName,attrNameLength,attrNameBufferSize);
                        attrValueLength=0;
                        HTML_PARSER_ADD_ATTRIBUTE;
                    }
                    HTML_PARSER_FINALIZE_TAG;
                    HTML_PARSER_NEXT_ITERATION;
                }
                if(thisChar=='=')
                {
                    isAttrName=false;
                    isAttrValue=true;
                    io::terminateBuffer(attrName,attrNameLength,attrNameBufferSize);
                    HTML_PARSER_NEXT_ITERATION;
                }
                if(thisChar=='<')
                {
                    HTML_PARSER_CHECK_IF_TAG_IS_VALID;
                    HTML_PARSER_NEXT_ITERATION;
                }
                if(thisChar=='/')
                {
                    isSelfClosingTag=true;
                    HTML_PARSER_NEXT_ITERATION;
                }
                if(attrNameHadWhitespace)
                {
                    // This is a boolean attribute.
                    io::terminateBuffer(attrName,attrNameLength,attrNameBufferSize);
                    HTML_PARSER_ADD_ATTRIBUTE;
                    io::writeRawCharToBuffer(attrName,thisChar,attrNameLength,attrNameBufferSize);
                    attrNameHadWhitespace=false;
                    HTML_PARSER_NEXT_ITERATION;
                }
                io::writeRawCharToBuffer(attrName,thisChar,attrNameLength,attrNameBufferSize);
                HTML_PARSER_NEXT_ITERATION;
            }
            if(isAttrValue)
            {
                if(isString)
                {
                    if(thisChar==stringChar&&prevNonWhitespaceChar!='\\')
                    {
                        isString=false;
                        if(!isDoctypeDeclaration)
                        {
                            isAttrValue=false;
                            isAttrName=true;
                            io::terminateBuffer(attrValue,attrValueLength,attrValueBufferSize);
                            HTML_PARSER_ADD_ATTRIBUTE;
                            HTML_PARSER_NEXT_ITERATION;
                        }
                    }
                    io::writeRawCharToBuffer(attrValue,thisChar,attrValueLength,attrValueBufferSize);
                    HTML_PARSER_NEXT_ITERATION;
                }
                if(isWhitespace&&!isDoctypeDeclaration)
                {
                    if(attrValueLength>0)
                    {
                        isAttrValue=false;
                        isAttrName=true;
                        io::terminateBuffer(attrValue,attrValueLength,attrValueBufferSize);
                        HTML_PARSER_ADD_ATTRIBUTE;
                    }
                    // Do not add white spaces to the beginning of attribute values
                    HTML_PARSER_NEXT_ITERATION;
                }
                if(thisChar=='"'||thisChar=='\'')
                {
                    isString=true;
                    tagHadAttrWithStringValue=true;
                    stringChar=thisChar;
                    if(isDoctypeDeclaration)
                        io::writeRawCharToBuffer(attrValue,thisChar,attrValueLength,attrValueBufferSize);
                    else
                        attrValueLength=0;
                    HTML_PARSER_NEXT_ITERATION;
                }
                if(thisChar=='>')
                {
                    io::terminateBuffer(attrValue,attrValueLength,attrValueBufferSize);
                    if(isDoctypeDeclaration)
                    {
                        Node *thisNode=openNodes[openNodeCollectionLength-1];
                        thisNode->valueLength=attrValueLength;
                        thisNode->valueBufferSize=attrValueBufferSize;
                        thisNode->value=text::duplicateString(attrValue);
                        isDoctypeDeclaration=false;
                        attrNameBufferSize=DEFAULT_NODE_PARSER_ATTR_NAME_BUFFER_SIZE;
                        attrNameLength=0;
                        free(attrName);
                        attrName=(char*)malloc(attrNameBufferSize);
                        attrValueBufferSize=DEFAULT_NODE_PARSER_ATTR_VALUE_BUFFER_SIZE;
                        attrValueLength=0;
                        free(attrValue);
                        attrValue=(char*)malloc(attrValueBufferSize);
                        isAttrValue=false;
                        HTML_PARSER_FINALIZE_TAG;
                        HTML_PARSER_NEXT_ITERATION;
                    }
                    isAttrValue=false;
                    HTML_PARSER_ADD_ATTRIBUTE;
                    HTML_PARSER_FINALIZE_TAG;
                    HTML_PARSER_NEXT_ITERATION;
                }
                if(thisChar=='<')
                {
                    HTML_PARSER_CHECK_IF_TAG_IS_VALID;
                    HTML_PARSER_NEXT_ITERATION;
                }
                if(thisChar=='/')
                {
                    isSelfClosingTag=true;
                    HTML_PARSER_NEXT_ITERATION;
                }
                io::writeRawCharToBuffer(attrValue,thisChar,attrValueLength,attrValueBufferSize);
                HTML_PARSER_NEXT_ITERATION;
            }

            // We are still at the tag name.

            if(isWhitespace)
            {
                if(tagNameLength>0)
                {
                    Node *thisNode=openNodes[openNodeCollectionLength-1];
                    isAttrName=true;
                    io::terminateBuffer(tagName,tagNameLength,tagNameBufferSize);
                    thisNode->name=text::duplicateString(tagName);
                    if(stricmp(tagName,"!doctype")==0)
                    {
                        thisNode->type=NODE_TYPE_DOCTYPE;
                        // Do not set thisNode->name to ""! Needed above!
                        isDoctypeDeclaration=true;
                        isAttrName=false;
                        isAttrValue=true;
                    }
                }
                HTML_PARSER_NEXT_ITERATION;
            }
            if(thisChar=='>')
            {
                io::terminateBuffer(tagName,tagNameLength,tagNameBufferSize);
                openNodes[openNodeCollectionLength-1]->name=text::duplicateString(tagName);
                HTML_PARSER_FINALIZE_TAG;
                HTML_PARSER_NEXT_ITERATION;
            }
            if(thisChar=='/')
            {
                if(tagNameLength==0)
                    isEndTag=true;
                else
                    isSelfClosingTag=true;
                HTML_PARSER_NEXT_ITERATION;
            }
            if(thisChar=='<')
            {
                HTML_PARSER_CHECK_IF_TAG_IS_VALID;
                HTML_PARSER_NEXT_ITERATION;
            }
            io::writeRawCharToBuffer(tagName,thisChar,tagNameLength,tagNameBufferSize);
            if(tagNameLength==3&&strncmp(tagName,"!--",tagNameLength)==0) // Note that strncmp assumes _both_ strings to be _Maxcount chars long, therefore we need to check the length.
            {
                Node *thisNode=openNodes[openNodeCollectionLength-1];
                thisNode->type=NODE_TYPE_COMMENT;
                // Allocate value buffer:
                thisNode->valueBufferSize=DEFAULT_NODE_PARSER_TEXT_CONTENT_BUFFER_SIZE; // Not an attribute value
                free(thisNode->value);
                thisNode->value=(char*)malloc(thisNode->valueBufferSize);
                NodeCollection::addNode(nodes,nodeCount,bufferSize,thisNode);
                openNodeCollectionLength--; // Remove this node. It is the last one.
                isComment=true;
                HTML_PARSER_CLEAR_BUFFERS; // We need this, and not HTML_PARSER_FINALIZE_TAG.
                HTML_PARSER_NEXT_ITERATION;
            }
            if(tagNameLength==8&&strncmp(tagName,"![CDATA[",tagNameLength)==0) // Note that strncmp assumes _both_ strings to be _Maxcount chars long, therefore we need to check the length.
            {
                Node *thisNode=openNodes[openNodeCollectionLength-1];
                thisNode->type=NODE_TYPE_CDATA;
                thisNode->valueBufferSize=DEFAULT_NODE_PARSER_TEXT_CONTENT_BUFFER_SIZE; // Not an attribute value
                free(thisNode->value);
                thisNode->value=(char*)malloc(thisNode->valueBufferSize);
                NodeCollection::addNode(nodes,nodeCount,bufferSize,thisNode);
                openNodeCollectionLength--; // Remove this node. It is the last one.
                isCDATA=true;
                HTML_PARSER_CLEAR_BUFFERS; // We need this, and not HTML_PARSER_FINALIZE_TAG.
                HTML_PARSER_NEXT_ITERATION;
            }
            HTML_PARSER_NEXT_ITERATION;
        }
        if(isComment) // Just like isCDATA
        {
            // Keep appending the chars to the value of nodes[nodeCount-1] until we find -->, then trim the value.
            Node *thisNode=nodes[nodeCount-1];
            if(thisChar=='>')
            {
                // endsWith cannot be used on a buffer!
                // Check if the last two characters were both -
                if(thisNode->valueLength>1&&thisNode->value[thisNode->valueLength-1]=='-'&&thisNode->value[thisNode->valueLength-2]=='-')
                {
                    char *newValue=text::firstChars(thisNode->value,thisNode->valueLength-2);
                    free(thisNode->value);
                    thisNode->value=newValue; // Just to be safe, if someone ignores thisNode->valueLength.
                    thisNode->valueLength-=2;
                    isComment=false;
                    HTML_PARSER_NEXT_ITERATION;
                }
            }
            io::writeRawCharToBuffer(thisNode->value,thisChar,thisNode->valueLength,thisNode->valueBufferSize);
            HTML_PARSER_NEXT_ITERATION;
        }
        // IMPORTANT NOTE: isPlaintext covers only style, script and plaintext nodes!
        // Text nodes are NOT handled here!
        if(isPlaintext) // Almost like the other two, except the length of the terminating string.
        {
            Node *thisNode=openNodes[openNodeCollectionLength-1];

            if(thisChar=='>')
            {
                // Special cases (bbc.com): "\x3Cscript>"; therefore, use "</script>"

                // Note that > has not been appended to nonWhitespaceString yet!
                // Append + 1 to remove >, too!
                char *cmpStr = nonWhitespaceString+nonWhitespaceStringLength-plaintextEndLen + 1;
                if(strnicmp(cmpStr,plaintextEnd,plaintextEndLen-1 /*Note that > has not been appended to nonWhitespaceString yet, hence -1 is needed!*/)==0)
                {
                    thisNode->valueLength=thisNode->valueLength-(i-lastOpeningTagPositionInPlaintext);
                    io::terminateBuffer(thisNode->value,thisNode->valueLength,thisNode->valueBufferSize);
                    isPlaintext=false;
                    // Do not remove this node from openNodes right after the opening tag ends, there will be child nodes!
                    // NOTE: Node gets added to "nodes" in HTML_PARSER_FINALIZE_TAG (called upon reaching the end of the opening tag).
                    openNodeCollectionLength--; // Remove this node. It is the last one.
                    // No HTML_PARSER_FINALIZE_TAG here!´
                    HTML_PARSER_CLEAR_BUFFERS;
                    HTML_PARSER_NEXT_ITERATION;
                }
            }

            if(thisChar=='<')
                lastOpeningTagPositionInPlaintext=i;

            io::writeRawCharToBuffer(thisNode->value,thisChar,thisNode->valueLength,thisNode->valueBufferSize);

            HTML_PARSER_NEXT_ITERATION;
        }
        if(isCDATA) // Just like isComment
        {
            // Keep appending the chars to the value of nodes[nodeCount-1] until we find ]]>, then trim the value.
            Node *thisNode=nodes[nodeCount-1];
            if(thisChar=='>')
            {
                // endsWith cannot be used on a buffer!
                // Check if the last two characters were both ]
                if(thisNode->valueLength>1&&thisNode->value[thisNode->valueLength-1]==']'&&thisNode->value[thisNode->valueLength-2]==']')
                {
                    char *newValue=text::firstChars(thisNode->value,thisNode->valueLength-2);
                    free(thisNode->value);
                    thisNode->value=newValue; // Just to be safe, if someone ignores thisNode->valueLength.
                    thisNode->valueLength-=2;
                    isCDATA=false;
                    HTML_PARSER_NEXT_ITERATION;
                }
            }
            io::writeRawCharToBuffer(thisNode->value,thisChar,thisNode->valueLength,thisNode->valueBufferSize);
            HTML_PARSER_NEXT_ITERATION;
        }
        if(thisChar=='<')
        {
            // Found a new non-text node
            HTML_PARSER_NEW_NODE;
            HTML_PARSER_NEXT_ITERATION;
        }
        if(!isTextNode) // Found a new text node
        {
            Node *n=new Node(nextNodeId++,NODE_TYPE_TEXT,text::duplicateString(""),0,text::duplicateString(""));
            n->parent=openNodes[openNodeCollectionLength-1]->id;
            NodeCollection::addChild(openNodes[openNodeCollectionLength-1],n->id);
            n->valueBufferSize=DEFAULT_NODE_PARSER_TEXT_CONTENT_BUFFER_SIZE;
            free(n->value);
            n->value=(char*)malloc(n->valueBufferSize);
            NodeCollection::addNode(nodes,nodeCount,bufferSize,n);
            isTextNode=true;
        }
        // Do not use openNodes here.
        char *val = nodes[nodeCount-1]->value;
        io::writeRawCharToBuffer(nodes[nodeCount-1]->value,thisChar,nodes[nodeCount-1]->valueLength,nodes[nodeCount-1]->valueBufferSize);
        HTML_PARSER_NEXT_ITERATION; // Do not delete this, this isn't just "continue".
    }

    // Finalize open attributes and tags

    if(isAttrName||isAttrValue)
    { // {} needed here.
        HTML_PARSER_ADD_ATTRIBUTE;
    }

    if(isTag) // Do not check if the tag is valid here, this could be something like <button selected
    { // {} needed here.
        HTML_PARSER_FINALIZE_TAG;
    }

    // Remove root node from openNodes

    NodeCollection::removeNode(openNodes,openNodeCollectionLength,openNodeBufferSize,rootNode);

    // Add all items still in openNodes (we do not need to remove them)

    for(node_id_t i=0;i<openNodeCollectionLength;i++)
        NodeCollection::addNode(nodes,nodeCount,bufferSize,openNodes[i]);
    free(openNodes);

    // Free buffers

    free(nonWhitespaceString);
    free(attrName);
    free(attrValue);
    free(tagName);
    free(plaintextEnd);

    // Undefine macros

    #undef HTML_PARSER_ADD_ATTRIBUTE
    #undef HTML_PARSER_NEXT_ITERATION
    #undef HTML_PARSER_NEW_NODE
    #undef HTML_PARSER_FINALIZE_TAG
    #undef HTML_PARSER_CLEAR_BUFFERS
    #undef HTML_PARSER_CHECK_IF_TAG_IS_VALID

    return nodes;
}

char *html::parser::serialize(html::Node **nodes, node_id_t size, html::Node *rootNode)
{
    fs_t pos=0;
    fs_t bufferSize=DEFAULT_HTML_SERIALIZER_BUFFER_SIZE;
    char *out=(char*)malloc(bufferSize);

    if(rootNode->type==NODE_TYPE_TEXT)
    {
        io::writeZeroTerminatedDataToBuffer(out,rootNode->value,pos,bufferSize);
        return out;
    }
    else if(rootNode->type==NODE_TYPE_ATTRIBUTE)
    {
        char *str=text::concat(rootNode->name,"=\"",text::escapeDoubleQuotationMarks(rootNode->value),"\"");
        io::writeZeroTerminatedDataToBuffer(out,str,pos,bufferSize);
        free(str);
        return out;
    }
    else if(rootNode->type==NODE_TYPE_COMMENT)
    {
        char *str=text::concat("<!--",rootNode->value,"-->");
        io::writeZeroTerminatedDataToBuffer(out,str,pos,bufferSize);
        free(str);
        return out;
    }
    else if(rootNode->type==NODE_TYPE_CDATA)
    {
        char *str=text::concat("<![CDATA[",rootNode->value,"]]>");
        io::writeZeroTerminatedDataToBuffer(out,str,pos,bufferSize);
        free(str);
        return out;
    }

    Node **children=NodeCollection::getChildren(nodes,size,rootNode);

    bool foundInner=false;
    bool isPlaintext=(rootNode->type&NODE_TYPE_PLAINTEXT)==NODE_TYPE_PLAINTEXT;
    bool isDoctypeDeclaration=rootNode->type==NODE_TYPE_DOCTYPE;
    bool foundHtml=false;
    if(rootNode->type&NODE_TYPE_ELEMENT)
    {
        char *add=text::concat("<",rootNode->name);
        io::writeRawDataToBuffer(out,add,strlen(add),pos,bufferSize);
        free(add);
        for(node_id_t i=0;i<rootNode->childCount;i++)
        {
            Node *thisChild=children[i];
            if(thisChild->type==NODE_TYPE_ATTRIBUTE)
            {
                char *esc=text::escapeDoubleQuotationMarks(thisChild->value);
                char *attrStr=text::concat(" ",thisChild->name,"=\"",esc,"\"");
                io::writeRawDataToBuffer(out,attrStr,strlen(attrStr),pos,bufferSize);
                free(esc);
                free(attrStr);
            }
            else
                foundInner=true;
        }
        if(!foundInner&&!isPlaintext) //  Plaintext nodes cannot be self-closing.
        {
            // We can't just use /> all the time.
            // Modify in "parse", too.
            if(stricmp(rootNode->name,"img")==0||
                    stricmp(rootNode->name,"br")==0||
                    stricmp(rootNode->name,"input")==0||
                    stricmp(rootNode->name,"link")==0||
                    stricmp(rootNode->name,"meta")==0||
                    stricmp(rootNode->name,"hr")==0||
                    stricmp(rootNode->name,"embed")==0||
                    stricmp(rootNode->name,"param")==0||
                    stricmp(rootNode->name,"area")==0||
                    stricmp(rootNode->name,"base")==0||
                    stricmp(rootNode->name,"source")==0||
                    stricmp(rootNode->name,"col")==0||
                    stricmp(rootNode->name,"command")==0)
            {
                io::writeRawDataToBuffer(out," /",2,pos,bufferSize);
            }
            else
            {
                char *endTag=text::concat("></",rootNode->name); // The closing > gets added below.
                io::writeRawDataToBuffer(out,endTag,strlen(endTag),pos,bufferSize);
                free(endTag);
            }
        }
        io::writeRawCharToBuffer(out,'>',pos,bufferSize);
    }
    if(isDoctypeDeclaration)
    {
        char *doctypeDeclaration=text::concat("<!DOCTYPE ",rootNode->value,">");
        io::writeRawDataToBuffer(out,doctypeDeclaration,strlen(doctypeDeclaration),pos,bufferSize);
        free(doctypeDeclaration);
    }
    if(isPlaintext) // Not "else if"! The condition below is true for DOCTYPE nodes!
    {
        if(rootNode->valueLength>0) // Keep this here, do not add this to the above condition!
        {
            io::writeRawDataToBuffer(out,rootNode->value,rootNode->valueLength,pos,bufferSize);
        }
        foundInner=true; // NOTE: This will add the closing tag. Plaintext nodes cannot be self-closing.
    }
    else if((rootNode->type==NODE_TYPE_ROOT)||(rootNode->type&NODE_TYPE_ELEMENT)||isDoctypeDeclaration)
    {
        // Serialize all child nodes that are not attributes.
        for(node_id_t i=0;i<rootNode->childCount;i++)
        {
            Node *thisChild=children[i];
            if(thisChild->type!=NODE_TYPE_ATTRIBUTE)
            {
                char *subser=serialize(nodes,size,thisChild);
                if(stricmp(thisChild->name,"html")==0)
                    foundHtml=true;
                io::writeRawDataToBuffer(out,subser,strlen(subser),pos,bufferSize);
            }
        }
    }
    if((rootNode->type&NODE_TYPE_ELEMENT)&&foundInner)
    {
        char *endTag=text::concat("</",rootNode->name,">");
        io::writeRawDataToBuffer(out,endTag,strlen(endTag),pos,bufferSize);
        free(endTag);
    }
    else if(isDoctypeDeclaration&&!foundHtml)
    {
        const char *htmlEndTag="</html>";
        io::writeRawDataToBuffer(out,htmlEndTag,strlen(htmlEndTag),pos,bufferSize);
    }
    io::terminateBuffer(out,pos,bufferSize);
    return out;
}
