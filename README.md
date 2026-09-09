# Usage
Uncomment **one** of the 'static char data' definitions in the huffmanEncoding.c file to test with that data.

The data will subsequently be encoded into a map and data, the size of both will be displayed.
The contents of the char-to-binary_code map will also be displayed.
Finally, the encoded data, along with the decoded output will be displayed along with a size reduction percentage.

There exists a counter-example choice for the 'static char data' (of some linked list code) that does **not** work well for Huffman Encoding.
The relevant errors are shown when the compressed data will be longer than the original (as occurs with the counter-example)

# Compilation & Run commands
Compile with any non-ancient C compiler and output, e.g.
```bash
gcc -o huffmanEncoding huffmanEncoding.c
./huffmanEncoding
```
Note: This code was written and compiled on Windows (a while ago) and tested on a Linux machine (just now).
