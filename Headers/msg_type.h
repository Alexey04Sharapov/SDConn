
enum msg_type {
    MSG_TYPE_UPDATE = 0, //It's an inner message. Android shouldn't use it.
    MSG_TYPE_PING
};

#define MSG_COUNT 2
static const int msg_count = MSG_COUNT;

typedef void (*callback_foo) (enum msg_type type, size_t bodySize, char const body [][bodySize],
    void (*send)(enum msg_type type, size_t bodySize, char const body [][bodySize], void (*callback)(int status)));
