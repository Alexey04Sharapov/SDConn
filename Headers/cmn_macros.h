
#define decltype(type) __typeof__(type)
#define auto_type __auto_type
#define are_type_compatible(type1, type2) __builtin_types_compatible_p((type1), (type2))

#define is_const(exp) __builtin_constant_p(exp)
#define choose_expr(exp, exp1, exp2)  __builtin_choose_expr((exp), (exp1), (exp2))

#define likely(x)       __builtin_expect(!!(x),1)
#define unlikely(x)     __builtin_expect(!!(x),0)

#define array_size(x) (sizeof(x) / sizeof((x)[0]))
#define field_sizeof(t, f) (sizeof(((t*)0)->f))

#define nullptr ((void *)0)

//In C2x mode shall be replaced [[gnu::attr]]
#define attribute(attr) __attribute__((attr))

