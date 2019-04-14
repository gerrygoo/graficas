TARGET_EXEC ?= run

BUILD_DIR ?= ./build
SRC_DIRS ?= ComputerGraphics Dependencies Math Util

SRCS := $(shell find $(SRC_DIRS) -type f -name *.cpp -or -name *.c -not -path "*/IL/*")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
SHADERS := $(wildcard ./ComputerGraphics/shaders/*)
BUILT_SHADERS := $(addprefix $(BUILD_DIR)/shaders/, $(notdir $(SHADERS)))
IMAGES := $(wildcard ./ComputerGraphics/images/*)
BUILT_IMAGES := $(addprefix $(BUILD_DIR)/images/, $(notdir $(IMAGES)))

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
# LDFLAGS += -llua

CPPFLAGS += $(INC_FLAGS) -MMD -MP
CXXFLAGS += -std=c++11
CFLAGS += -std=c11

LDFLAGS += -lglfw -L/usr/local/Cellar/devil/1.8.0_1/lib -lIL -lILU

.PHONY: all
all: $(BUILD_DIR)/$(TARGET_EXEC) $(BUILT_SHADERS) $(BUILT_IMAGES)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# shaders
$(BUILD_DIR)/shaders/%: ComputerGraphics/shaders/%
	$(MKDIR_P) $(dir $@)
	cp $< $@

# images
$(BUILD_DIR)/images/%: ComputerGraphics/images/%
	$(MKDIR_P) $(dir $@)
	cp $< $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) $(TARGET_EXEC)

-include $(DEPS)

MKDIR_P ?= mkdir -p
