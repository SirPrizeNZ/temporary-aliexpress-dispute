/* Read-only AArch64 storage measurement. JNI table indices verified against jni.h. */
typedef long long jlong;
struct filesystem {
    long type, block_size;
    unsigned long blocks, free_blocks, available_blocks, files, free_files;
    int fsid[2];
    long name_length, fragment_size, flags, spare[4];
};
extern int statfs(const char *, struct filesystem *);
extern long syscall(long, ...);
__attribute__((visibility("default"))) void *Java_local_watchaudit_MainActivity_rawStorage(void *environment, void *object) {
    void **functions = *(void ***)environment;
    void *(*new_array)(void *, int) = (void *(*)(void *, int))functions[180];
    void (*set_region)(void *, void *, int, int, const jlong *) = (void (*)(void *, void *, int, int, const jlong *))functions[212];
    struct filesystem normal = {0}, direct = {0};
    jlong values[8];
    values[0] = statfs("/data", &normal);
    values[1] = normal.blocks * normal.block_size;
    values[2] = normal.available_blocks * normal.block_size;
    values[3] = normal.free_blocks * normal.block_size;
    /* Linux AArch64 syscall 43 is statfs. No writes or privileged operations. */
    values[4] = syscall(43, "/data", &direct);
    values[5] = direct.blocks * direct.block_size;
    values[6] = direct.available_blocks * direct.block_size;
    values[7] = direct.free_blocks * direct.block_size;
    void *array = new_array(environment, 8);
    if (array) set_region(environment, array, 0, 8, values);
    return array;
}
