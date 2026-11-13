XWIN       ?= $(HOME)/Library/Caches/cargo-xwin/xwin
LLVM       ?= $(shell brew --prefix llvm)
CXX         = $(LLVM)/bin/clang-cl
LINK        = lld-link
RC          = $(LLVM)/bin/llvm-rc

OUTDIR      = x64/Release
INTDIR      = x64/Release/obj

CXXFLAGS    = /std:c++20 /EHsc /O2 /MT /Zc:__cplusplus \
              /DNDEBUG /DGW2LOGPROOFS_EXPORTS /D_WINDOWS /D_USRDLL /D_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR \
              -imsvc "$(XWIN)/crt/include" \
              -imsvc "$(XWIN)/sdk/include/ucrt" \
              -imsvc "$(XWIN)/sdk/include/um" \
              -imsvc "$(XWIN)/sdk/include/shared" \
              -Isrc \
              --target=x86_64-pc-windows-msvc

LDFLAGS     = /DLL /SUBSYSTEM:WINDOWS /OPT:REF /OPT:ICF \
              /LIBPATH:"$(XWIN)/crt/lib/x86_64" \
              /LIBPATH:"$(XWIN)/sdk/lib/um/x86_64" \
              /LIBPATH:"$(XWIN)/sdk/lib/ucrt/x86_64" \
              shell32.lib

SRCS        = $(shell sed -n 's/.*<ClCompile Include="\([^"]*\)".*/\1/p' GW2LogProofs.vcxproj | tr '\\' '/')
OBJS        = $(addprefix $(INTDIR)/,$(SRCS:.cpp=.obj))
RC_OBJ      = $(INTDIR)/resource.res
TARGET      = $(OUTDIR)/nexus_log_proofs.dll

all: version $(TARGET)

version:
	@year=$$(date -u +%Y); month=$$(date -u +%-m); day=$$(date -u +%-d); \
	hour=$$(date -u +%-H); min=$$(date -u +%-M); rev=$$((hour * 60 + min)); \
	printf '#pragma once\n#define V_MAJOR %s\n#define V_MINOR %s\n#define V_BUILD %s\n#define V_REVISION %s\n' \
		$$year $$month $$day $$rev > src/version.h

# winres.h ships with MFC, which xwin does not download; winresrc.h is its Windows SDK equivalent.
$(RC_OBJ): src/resource.rc src/resource.h
	@mkdir -p $(dir $@)
	iconv -f UTF-16LE -t UTF-8 $< | sed -e 's/winres\.h/winresrc.h/' -e 's/\\\\/\//g' > src/resource_utf8.rc
	$(RC) /fo $@ /I src /I "$(XWIN)/sdk/include/um" /I "$(XWIN)/sdk/include/shared" src/resource_utf8.rc; \
		status=$$?; rm -f src/resource_utf8.rc; exit $$status

$(INTDIR)/%.obj: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) /c $(CXXFLAGS) /clang:-MMD /clang:-MP /clang:-MF$(@:.obj=.d) /Fo$@ $<

$(TARGET): $(OBJS) $(RC_OBJ)
	@mkdir -p $(OUTDIR)
	$(LINK) $(LDFLAGS) /OUT:$@ $(OBJS) $(RC_OBJ)

clean:
	rm -rf $(INTDIR) $(OUTDIR)

.PHONY: all clean version

-include $(OBJS:.obj=.d)
