#include <string>
#include <tidy/tidy.h> //-ltidy
#include <tidy/buffio.h>

namespace ufp {

	inline std::string CleanHtml(const std::string&);

	std::string CleanHtml(const std::string &in) {
		TidyDoc tidyDoc = tidyCreate();
		TidyBuffer tidyOutputBuffer = {0};
		tidyOptSetBool(tidyDoc, TidyXmlOut, yes);
		tidyOptSetBool(tidyDoc, TidyQuiet, yes);
		tidyOptSetInt(tidyDoc, TidyShowErrors, 0);
		tidyOptSetBool(tidyDoc, TidyForceOutput, yes);
		tidyOptSetBool(tidyDoc, TidyNumEntities, yes);
		tidyOptSetBool(tidyDoc, TidyShowWarnings, no);
		tidySetCharEncoding(tidyDoc, "utf8");
		tidyParseString(tidyDoc, in.c_str());
		tidyCleanAndRepair(tidyDoc);
		tidySaveBuffer(tidyDoc, &tidyOutputBuffer);
		std::string tidyResult = (char*)tidyOutputBuffer.bp;
		tidyBufFree(&tidyOutputBuffer);
		tidyRelease(tidyDoc);
		return tidyResult;
	}

}