/* Data that we keep for a file-based handle. */
typedef struct {
    /* File descriptor. */
    int fd;

    /* Is it seekable? */
    short seekable;

    /* Is it know to be writable? */
    short known_writable;

    /* How many bytes have we read/written? Used to fake tell on handles that
     * are not seekable. */
    MVMint64 byte_position;

    /* Did read already report EOF? */
    int eof_reported;

    /* Output buffer, for buffered output. */
    char *output_buffer;

    /* Size of the output buffer, for buffered output; 0 if not buffering. */
    size_t output_buffer_size;

    /* How much of the output buffer has been used so far. */
    size_t output_buffer_used;
} MVMIOFileData;

MVMObject * MVM_file_open_fh(MVMThreadContext *tc, MVMString *filename, MVMString *mode);
MVMObject * MVM_file_handle_from_fd(MVMThreadContext *tc, uv_file fd);
