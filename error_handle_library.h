
#define CONDITION_RETURN(condition, error_code) \
	if (__builtin_expect(condition,0)) {    \
		return error_code;              \
	}

#define CONDITION_FINALIZATION(condition, err_var, error_code) \
	if (__builtin_expect(condition,0)) {     \
		err_var = error_code;                \
		goto finalization;               \
	}
