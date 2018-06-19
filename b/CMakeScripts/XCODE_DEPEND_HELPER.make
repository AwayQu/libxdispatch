# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.xdispatch.Debug:
/Users/away/code/github/testxdispatch/b/bin/Debug/xdispatch.framework/xdispatch:
	/bin/rm -f /Users/away/code/github/testxdispatch/b/bin/Debug/xdispatch.framework/xdispatch


PostBuild.xdispatch_tests.Debug:
PostBuild.xdispatch.Debug: /Users/away/code/github/testxdispatch/b/bin/Debug/xdispatch_testsD
PostBuild.munit.Debug: /Users/away/code/github/testxdispatch/b/bin/Debug/xdispatch_testsD
/Users/away/code/github/testxdispatch/b/bin/Debug/xdispatch_testsD:\
	/Users/away/code/github/testxdispatch/b/bin/Debug/xdispatch.framework/Versions/0.8.0~devel/xdispatch\
	/Users/away/code/github/testxdispatch/b/bin/Debug/libmunitD.a
	/bin/rm -f /Users/away/code/github/testxdispatch/b/bin/Debug/xdispatch_testsD


PostBuild.munit.Debug:
/Users/away/code/github/testxdispatch/b/bin/Debug/libmunitD.a:
	/bin/rm -f /Users/away/code/github/testxdispatch/b/bin/Debug/libmunitD.a


PostBuild.xdispatch.Release:
/Users/away/code/github/testxdispatch/b/bin/Release/xdispatch.framework/xdispatch:
	/bin/rm -f /Users/away/code/github/testxdispatch/b/bin/Release/xdispatch.framework/xdispatch


PostBuild.xdispatch_tests.Release:
PostBuild.xdispatch.Release: /Users/away/code/github/testxdispatch/b/bin/Release/xdispatch_tests
PostBuild.munit.Release: /Users/away/code/github/testxdispatch/b/bin/Release/xdispatch_tests
/Users/away/code/github/testxdispatch/b/bin/Release/xdispatch_tests:\
	/Users/away/code/github/testxdispatch/b/bin/Release/xdispatch.framework/Versions/0.8.0~devel/xdispatch\
	/Users/away/code/github/testxdispatch/b/bin/Release/libmunit.a
	/bin/rm -f /Users/away/code/github/testxdispatch/b/bin/Release/xdispatch_tests


PostBuild.munit.Release:
/Users/away/code/github/testxdispatch/b/bin/Release/libmunit.a:
	/bin/rm -f /Users/away/code/github/testxdispatch/b/bin/Release/libmunit.a


PostBuild.xdispatch.MinSizeRel:
/Users/away/code/github/testxdispatch/b/bin/MinSizeRel/xdispatch.framework/xdispatch:
	/bin/rm -f /Users/away/code/github/testxdispatch/b/bin/MinSizeRel/xdispatch.framework/xdispatch


PostBuild.xdispatch_tests.MinSizeRel:
PostBuild.xdispatch.MinSizeRel: /Users/away/code/github/testxdispatch/b/bin/MinSizeRel/xdispatch_tests
PostBuild.munit.MinSizeRel: /Users/away/code/github/testxdispatch/b/bin/MinSizeRel/xdispatch_tests
/Users/away/code/github/testxdispatch/b/bin/MinSizeRel/xdispatch_tests:\
	/Users/away/code/github/testxdispatch/b/bin/MinSizeRel/xdispatch.framework/Versions/0.8.0~devel/xdispatch\
	/Users/away/code/github/testxdispatch/b/bin/MinSizeRel/libmunit.a
	/bin/rm -f /Users/away/code/github/testxdispatch/b/bin/MinSizeRel/xdispatch_tests


PostBuild.munit.MinSizeRel:
/Users/away/code/github/testxdispatch/b/bin/MinSizeRel/libmunit.a:
	/bin/rm -f /Users/away/code/github/testxdispatch/b/bin/MinSizeRel/libmunit.a


PostBuild.xdispatch.RelWithDebInfo:
/Users/away/code/github/testxdispatch/b/bin/RelWithDebInfo/xdispatch.framework/xdispatch:
	/bin/rm -f /Users/away/code/github/testxdispatch/b/bin/RelWithDebInfo/xdispatch.framework/xdispatch


PostBuild.xdispatch_tests.RelWithDebInfo:
PostBuild.xdispatch.RelWithDebInfo: /Users/away/code/github/testxdispatch/b/bin/RelWithDebInfo/xdispatch_tests
PostBuild.munit.RelWithDebInfo: /Users/away/code/github/testxdispatch/b/bin/RelWithDebInfo/xdispatch_tests
/Users/away/code/github/testxdispatch/b/bin/RelWithDebInfo/xdispatch_tests:\
	/Users/away/code/github/testxdispatch/b/bin/RelWithDebInfo/xdispatch.framework/Versions/0.8.0~devel/xdispatch\
	/Users/away/code/github/testxdispatch/b/bin/RelWithDebInfo/libmunit.a
	/bin/rm -f /Users/away/code/github/testxdispatch/b/bin/RelWithDebInfo/xdispatch_tests


PostBuild.munit.RelWithDebInfo:
/Users/away/code/github/testxdispatch/b/bin/RelWithDebInfo/libmunit.a:
	/bin/rm -f /Users/away/code/github/testxdispatch/b/bin/RelWithDebInfo/libmunit.a




# For each target create a dummy ruleso the target does not have to exist
/Users/away/code/github/testxdispatch/b/bin/Debug/libmunitD.a:
/Users/away/code/github/testxdispatch/b/bin/Debug/xdispatch.framework/Versions/0.8.0~devel/xdispatch:
/Users/away/code/github/testxdispatch/b/bin/MinSizeRel/libmunit.a:
/Users/away/code/github/testxdispatch/b/bin/MinSizeRel/xdispatch.framework/Versions/0.8.0~devel/xdispatch:
/Users/away/code/github/testxdispatch/b/bin/RelWithDebInfo/libmunit.a:
/Users/away/code/github/testxdispatch/b/bin/RelWithDebInfo/xdispatch.framework/Versions/0.8.0~devel/xdispatch:
/Users/away/code/github/testxdispatch/b/bin/Release/libmunit.a:
/Users/away/code/github/testxdispatch/b/bin/Release/xdispatch.framework/Versions/0.8.0~devel/xdispatch:
