#include <iostream>
#include <vector>
#include <stdlib.h>
#include "text.h"
#include "ini.h"
#include "io.h"
#include "config.h"
#include "json.h"
#include "win.h"
#include "winnet.h"
#include "html.h"

#include <Windows.h>

using namespace std;
using namespace html;

int main()
{

    uint32_t size;
    char *source=winnet::downloadData("http://google.com/",size);
    puts("Downloaded!");
    source=text::firstChars(source,size);
    win::writeFile("C:\\Test\\source.html",source,strlen(source));
    node_id_t nodeCount;
    node_id_t bufferSize;
    Node *rootNode;
    Node **nodes=parser::parse(source,nodeCount,bufferSize,rootNode);
    puts("Parsed!");
    char *output=parser::serialize(nodes,nodeCount,rootNode);
    puts("Serialized!");
    win::writeFile("C:\\Test\\output.html",output,strlen(output));
    return 0;
}

