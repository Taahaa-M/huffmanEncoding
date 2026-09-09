#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


typedef unsigned long long bignum_t;

typedef struct {
    int character; // yes, i know
    // i think i use them as indices at some point
    bignum_t count;
} CharData;

typedef struct CharNode CharNode;
struct CharNode {
    char character;
    CharNode* right;
    CharNode* left;
};

typedef struct {
    uint64_t code;
    size_t len;
} BinCode;


#define CHAR_MAX 256
#define MAX_STRING_SIZE 2048


// a few strings to try out
static char data[] = "\n"
"                    __________                                   \n"
"                  .'----------`.                                 \n"
"                  | .--------. |                                 \n"
"                  | |@@@@@@@@| |       __________                \n"
"                  | |@@@@@@@@| |      /__________\\               \n"
"         .--------| `--------' |------|    --=-- |-------------. \n"
"         |        `----,-.-----'      |o ======  |             | \n"
"         |       ______|_|_______     |__________|             | \n"
"         |      / %%%%%%%%%%%%%% \\                             | \n"
"         |     / %%%%%%%%%%%%%%%% \\                            | \n"
"         |     ^^^^^^^^^^^^^^^^^^^^                            | \n"
"         +-----------------------------------------------------+ \n"
"         ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ \n";

/* static char data[] = "\n"
"             O\n"
"            (_)\n"
"          _ )_( _\n"
"        /`_) H (_`\\\n"
"      .' (  { }  ) '.\n"
"    _/ /` '-'='-' `\\ \\_\n"
"   [_.'   _,...,_   '._]\n"
"    |   .:\"`````\":.   |\n"
"    |__//_________\\__|\n"
"     | .-----------. |\n"
"     | |  .-\"\"\"-.  | |\n"
"     | | /    /  \\ | |\n"
"     | ||-   <   -|| |\n"
"     | | \\    \\  / | |\n"
"     | |[`'-...-'`]| |\n"
"     | | ;-.___.-; | |\n"
"     | | |  |||  | | |\n"
"     | | |  |||  | | |\n"
"     | | |  |||  | | |\n"
"     | | |  |||  | | |\n"
"     | | |  |||  | | |\n"
"     | | | _|||_ | | |\n"
"     | | | >===< | | |\n"
"     | | | |___| | | |\n"
"     | | |  |||  | | |\n"
"     | | |  ;-;  | | |\n"
"     | | | (   ) | | |\n"
"     | | |  '-'  | | |\n"
"     | | '-------' | |\n"
"jgs _| '-----------' |_\n"
"   [= === === ==== == =]\n"
"   [__--__--___--__--__]\n"
"  /__-___-___-___-___-__\\\n"
" `\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"`\n";*/

// static char data[] = "                                                               ";


// this string shows that code errs when encoding
// data that is not suitable for huffman encoding
// i.e. data that doesn't mostly use the same few characters.
// it encodes correctly, until space has run out
/* static char data[] = "#include <stdio.h>\n"
    "#include <stdlib.h>\n"
    "typedef struct node node;\n"
    "\n"
    "struct node {\n"
    "   int val;\n"
    "   node* next;\n"
    "};\n"
    "\n"
    "void traverse(node* head);\n"
    "\n"
    "void freeList(node* head);\n"
    "\n"
    "int addNode(int val, node** head);\n"
    "\n"
    "int deleteNode(int val, node **head);\n"
    "\n"
    "int insertByIdx(int idx, int val, node** head);\n"
    "\n"
    "int getLength(node* head);\n"
    "int main(void) {\n"
    "   node* linkedList = NULL;\n"
    "   int errSum = 0;\n"
    "\n"
    "   errSum += addNode(1, &linkedList);\n"
    "   errSum += addNode(2, &linkedList);\n"
    "\n"
    "   if (errSum < 0) {\n"
    "       return -1;\n"
    "   }\n"
    "\n"
    "   printf(\"traverse 1: \\n\");\n"
    "   traverse(linkedList);\n"
    "   printf(\"Length: %d\\n\\n\", getLength(linkedList));\n"
    "   errSum += addNode(3, &linkedList);\n"
    "   errSum += addNode(5, &linkedList);\n"
    "   \n"
    "   if (errSum < 0) {\n"
    "       return -1;\n"
    "   }\n"
    "\n"
    "   printf(\"traverse 2: \n\");\n"
    "   traverse(linkedList);\n"
    "   printf(\"Length: %d\n\n\", getLength(linkedList));\n"
    "\n"; */


int countChars(char data[], int charsData[CHAR_MAX], bignum_t lenData) {
    int distinctCharCount = 0;
    for (int i = 0; i < lenData; i++) {
        if (charsData[data[i]] == 0) {
            distinctCharCount++;
        }

        charsData[data[i]]++;
    }

    return distinctCharCount;
}


void recordCharData(int charsData[CHAR_MAX], CharData chars[]) {
    int distinctCharIdx = 0;
    for (int i = 0; i < CHAR_MAX; i++) {
        if (charsData[i] > 0) {
            chars[distinctCharIdx].character = i;
            chars[distinctCharIdx].count = charsData[i];
            distinctCharIdx++;
        }
    }
}


int comparator(const void* char1, const void* char2) {
    bignum_t val1 = ((CharData*)char1)->count;
    bignum_t val2 = ((CharData*)char2)->count;

    // so i can safely return integer, without overflow
    // as opposed to 'return val1 - val2;'
    if (val1 > val2) {
        return 1;
    } else if (val1 == val2) {
        return 0;
    } else {
        return -1;
    }
}


// for debugging
void visibleChar(char outputStr[6], int inputChar) {
    switch ((char)inputChar) {
        case ' ':
            strcpy(outputStr, "Space"); break;
        case '\n':
            strcpy(outputStr, "\\n"); break;
        case '\"':
            strcpy(outputStr, "\\\""); break;
        case '\\':
            strcpy(outputStr, "\\"); break;
        case '\'':
            strcpy(outputStr, "\\'"); break;
        case '\t':
            strcpy(outputStr, "\\t"); break;
        case '\0':
            strcpy(outputStr, "\\0"); break;
        default:
            outputStr[0] = inputChar;
            outputStr[1] = '\0';
            break;
    }
}


void createCharMap(char dest[], BinCode codes[], int n) {
    for (int i = 0; i < CHAR_MAX; i++) {
        if (codes[i].len > 0) {
            dest[codes[i].len - 1] = i;
        }
    }
}


void addCode(uint8_t encodedData[], BinCode c, bignum_t* idx, int* bitIdx) {
    uint8_t bitOffset;
    const uint8_t ones = ~0;

    // loop below removes chunks of uint8 blocks until a code of
    // smaller size than the bits left in encodedData[*idx] remains.
    // This is then handled by ending code
    while (c.len > sizeof(uint8_t)*8 - *bitIdx) {
        // this bitOffset goes rightward, to skip bits
        // that have already been set
        bitOffset = c.len + *bitIdx - sizeof(uint8_t) * 8;
        // bitOffset %= sizeof(uint8_t) * 8; - probs should be deleted?

        encodedData[*idx] |= c.code >> bitOffset;
        // rightshift to remove all bits that exceed size in current uint8

        c.len -= sizeof(uint8_t) * 8 - *bitIdx;
        // this effectively gets rid of used bits

        *bitIdx = 0;

        (*idx)++;
    }

    // this bitOffset goes leftward to insert bits
    // to leftmost bit place
    bitOffset = sizeof(uint8_t)*8 - *bitIdx - c.len;

    encodedData[*idx] |= c.code << bitOffset;

    *bitIdx += c.len;

    if (*bitIdx == sizeof(uint8_t)*8) {
        *bitIdx = 0;
        (*idx)++;
    }
}


uint8_t* encodeData(char data[], BinCode codes[CHAR_MAX], bignum_t lenData, bignum_t* lenEncodedData) {
    BinCode code;
    bignum_t workingOutputIdx = 0;
    int workingOutputBitIdx = 0; // idx of next free bit in byte - max 7

    uint8_t encodedDataArray[lenData]; // if it doesn't fit in this, don't encode?

    // initialise array with 0s
    for (bignum_t i = 0; i < lenData; i++) {
        encodedDataArray[i] = 0;
    }

    for (bignum_t i = 0; i < lenData && workingOutputIdx < lenData; i++) {
        code = codes[data[i]];
        addCode(encodedDataArray, code, &workingOutputIdx, &workingOutputBitIdx);
        // indices are updated in here ^
    }

    *lenEncodedData = workingOutputIdx + 1;
    if (workingOutputBitIdx == 0) {
        (*lenEncodedData)--;
    }

    uint8_t *encodedData = (uint8_t*) malloc((*lenEncodedData) * sizeof(uint8_t));
    memcpy(encodedData, encodedDataArray, *lenEncodedData);

    return encodedData;
}


void decodeData(char dest[], uint8_t encodedData[], bignum_t lenEncodedData, char charMap[], bignum_t* lenDecodedData) {
    uint8_t counter = 0;
    bignum_t destIdx = 0;

    bignum_t idx;
    int bitIdx;

    for (idx = 0; idx < lenEncodedData; idx++) {
        for (bitIdx = sizeof(uint8_t) * 8 - 1; bitIdx >= 0; bitIdx--) {
            if (((encodedData[idx] >> bitIdx) & 1) == 1) {
                dest[destIdx] = charMap[counter];

                destIdx++;
                counter = 0;
            } else {
                counter++;
            }
        }
    }

    *lenDecodedData = destIdx + 1;

    dest[destIdx] = '\0';
}


int findIdx(char tree[], bignum_t lenTree, char desired) {
    for (int i = 0; i < lenTree; i++) {
        if (tree[i] == desired) {
            return i;
        }
    }

    fprintf(stderr, "Failed to find index for %c\n", desired);
    return -1;
}


CharNode* buildHeap(CharData arr[], int n) {
    qsort(arr, n, sizeof(CharData), comparator);

    CharNode* val1 = malloc(sizeof(CharNode));
    CharNode* val2 = malloc(sizeof(CharNode));

    val1->character = arr[0].character;
    val2->character = arr[1].character;

    CharNode* sum = malloc(sizeof(CharNode));
    sum->right = val2;
    sum->left = val1;

    for (int i = 2; i < n; i++) {
        val1 = malloc(sizeof(CharNode));
        val1->character = arr[i].character;

        val2 = sum;

        sum = malloc(sizeof(CharNode));
        sum->right = val1;
        sum->left = val2;
    }

    return sum;
}


void getCodes(CharNode* heap, BinCode arr[CHAR_MAX], uint64_t binCode, int len) {
    if (heap->right == NULL && heap->left == NULL) {
        if (binCode == 0) {
            len++;
            binCode = 1; // prevent 0s with no ending 1 - case of ending left node
        }
        arr[heap->character].code = binCode;
        arr[heap->character].len = len;
    } else {
        binCode <<= 1; // shift bits left once - make space for next bit
        len++;
        
        if (heap->right != NULL) {
            getCodes(heap->right, arr, binCode | 1, len); // switch last bit on to indicate right node
        }

        if (heap->left != NULL) {
            getCodes(heap->left, arr, binCode, len); // left-shift causes last bit to be 0
        }
    }
}


void freeHeap(CharNode* heap) {
    if (heap->right != NULL) {
        freeHeap(heap->right);
    }

    if (heap->left != NULL) {
        freeHeap(heap->left);
    }

    free(heap);
}


void printBinary(BinCode c) {
    for (int i = c.len - 1; i >= 0; i--) {
        printf("%d", (c.code >> i) & 1 ? 1 : 0);
    }
}


void printEncodedData(uint8_t encodedData[], bignum_t lenEncodedData) {
    BinCode c;
    for (bignum_t i = 0; i < lenEncodedData; i++) {
        c.code = encodedData[i];
        c.len = sizeof(uint8_t) * 8;
        printBinary(c);
    }
    printf("\n");
}


void printCodes(BinCode arr[CHAR_MAX]) {
    char strbuff[6];
    for (int i = 0; i < CHAR_MAX; i++) {
        if (arr[i].len > 0) {
            visibleChar(strbuff, i);
            printf("%s: ", strbuff);
            printBinary(arr[i]);
            printf("\n");
        }
    }
}


int main(void) {
    bignum_t lenData = sizeof(data)/sizeof(char);
    printf("Initial size: %lu bytes\n\n", sizeof(data));

    int charsData[CHAR_MAX] = {};
    int distinctCharCount = countChars(data, charsData, lenData);

    CharData chars[distinctCharCount];
    recordCharData(charsData, chars);

    CharNode* heap = buildHeap(chars, distinctCharCount);

    BinCode codes[CHAR_MAX] = {};
    getCodes(heap, codes, 0, 0);
    printCodes(codes);

    char charMap[distinctCharCount];
    createCharMap(charMap, codes, distinctCharCount);
    printf("Map size: %lu bytes\n", sizeof(charMap));

    bignum_t lenEncodedData = 0;
    uint8_t* encodedData = encodeData(data, codes, lenData, &lenEncodedData);
    printf("Encoded Data size: %llu bytes\n\n", lenEncodedData * sizeof(uint8_t));
    printEncodedData(encodedData, lenEncodedData);

    double percentChange = 100.0 * (sizeof(data) - sizeof(charMap) - lenEncodedData * sizeof(uint8_t)) / sizeof(data);
    printf("Reduced size by %.1f%%\n\n", percentChange);

    char buffer[MAX_STRING_SIZE];
    bignum_t lenDecodedData;
    decodeData(buffer, encodedData, lenEncodedData, charMap, &lenDecodedData);

    printf("%s", buffer);

    free(encodedData);

    freeHeap(heap);

    return 0;
}
