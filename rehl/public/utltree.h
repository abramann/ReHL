

// Same as CaselessStringLessThan, but it ignores differences in / and \.
inline bool CaselessStringLessThanIgnoreSlashes(const char * const &lhs, const char * const &rhs)
{
	const char *pa = lhs;
	const char *pb = rhs;
	while (*pa && *pb)
	{
		char a = *pa;
		char b = *pb;

		// Check for dir slashes.
		if (a == '/' || a == '\\')
		{
			if (b != '/' && b != '\\')
				return ('/' < b);
		}
		else
		{
			if (a >= 'a' && a <= 'z')
				a = 'A' + (a - 'a');

			if (b >= 'a' && b <= 'z')
				b = 'A' + (b - 'a');

			if (a > b)
				return false;
			else if (a < b)
				return true;
		}
		++pa;
		++pb;
	}

	// Filenames also must be the same length.
	if (*pa != *pb)
	{
		// If pa shorter than pb then it's "less"
		return (!*pa);
	}

	return false;
}
