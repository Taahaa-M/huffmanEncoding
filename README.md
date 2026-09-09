#Usage
Uncomment one of the 'static char data' definitions in the huffmanEncoding.c file to test with that data.

The data will subsequently be encoded and then decoded, finally printing the output.

There exists a counter-example choice for the 'static char data' (of some linked list code) that does **not** work well for Huffman Encoding.
The relevant errors are shown when the compressed data will be longer than the original (as occurs with the counter-example)

#Compilation & Run commands
Compile with any non-ancient C compiler and output, e.g.
```bash
gcc -o huffmanEncoding huffmanEncoding.c
./huffmanEncoding
```
