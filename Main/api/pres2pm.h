
/*
    @type - type of message
    @bodySize - size (in bytes) of body
    @body - a pointer to the array, which contains an unencrypted payload.
    @callback - if you don't need to know the result (success or not), use
              nullptr as the argument. Otherwise, it will be called the callback,
              which contain the status. The status shall be 0 on success,
              otherwise non 0;
*/

void send_message(enum msg_type type, size_t bodySize, char const body [][bodySize],
        void (*callback)(int status));

void subscribe(void (*foo)(enum msg_type type, size_t bodySize, char const body [][bodySize]));