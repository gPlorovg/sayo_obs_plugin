// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: sayo.proto
// Protobuf C++ Version: 5.26.1

#include "sayo.pb.h"

#include <algorithm>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/extension_set.h"
#include "google/protobuf/wire_format_lite.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/reflection_ops.h"
#include "google/protobuf/wire_format.h"
#include "google/protobuf/generated_message_tctable_impl.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"
PROTOBUF_PRAGMA_INIT_SEG
namespace _pb = ::google::protobuf;
namespace _pbi = ::google::protobuf::internal;
namespace _fl = ::google::protobuf::internal::field_layout;
namespace sayo {

inline constexpr PingResponse::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : message_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        _cached_size_{0} {}

template <typename>
PROTOBUF_CONSTEXPR PingResponse::PingResponse(::_pbi::ConstantInitialized)
    : _impl_(::_pbi::ConstantInitialized()) {}
struct PingResponseDefaultTypeInternal {
  PROTOBUF_CONSTEXPR PingResponseDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~PingResponseDefaultTypeInternal() {}
  union {
    PingResponse _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 PingResponseDefaultTypeInternal _PingResponse_default_instance_;
      template <typename>
PROTOBUF_CONSTEXPR PingRequest::PingRequest(::_pbi::ConstantInitialized) {}
struct PingRequestDefaultTypeInternal {
  PROTOBUF_CONSTEXPR PingRequestDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~PingRequestDefaultTypeInternal() {}
  union {
    PingRequest _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 PingRequestDefaultTypeInternal _PingRequest_default_instance_;

inline constexpr AudioChunk::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : pcm_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        _cached_size_{0} {}

template <typename>
PROTOBUF_CONSTEXPR AudioChunk::AudioChunk(::_pbi::ConstantInitialized)
    : _impl_(::_pbi::ConstantInitialized()) {}
struct AudioChunkDefaultTypeInternal {
  PROTOBUF_CONSTEXPR AudioChunkDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~AudioChunkDefaultTypeInternal() {}
  union {
    AudioChunk _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 AudioChunkDefaultTypeInternal _AudioChunk_default_instance_;

inline constexpr ASRResult::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : text_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        _cached_size_{0} {}

template <typename>
PROTOBUF_CONSTEXPR ASRResult::ASRResult(::_pbi::ConstantInitialized)
    : _impl_(::_pbi::ConstantInitialized()) {}
struct ASRResultDefaultTypeInternal {
  PROTOBUF_CONSTEXPR ASRResultDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~ASRResultDefaultTypeInternal() {}
  union {
    ASRResult _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 ASRResultDefaultTypeInternal _ASRResult_default_instance_;
}  // namespace sayo
static ::_pb::Metadata file_level_metadata_sayo_2eproto[4];
static constexpr const ::_pb::EnumDescriptor**
    file_level_enum_descriptors_sayo_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_sayo_2eproto = nullptr;
const ::uint32_t
    TableStruct_sayo_2eproto::offsets[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
        protodesc_cold) = {
        ~0u,  // no _has_bits_
        PROTOBUF_FIELD_OFFSET(::sayo::AudioChunk, _internal_metadata_),
        ~0u,  // no _extensions_
        ~0u,  // no _oneof_case_
        ~0u,  // no _weak_field_map_
        ~0u,  // no _inlined_string_donated_
        ~0u,  // no _split_
        ~0u,  // no sizeof(Split)
        PROTOBUF_FIELD_OFFSET(::sayo::AudioChunk, _impl_.pcm_),
        ~0u,  // no _has_bits_
        PROTOBUF_FIELD_OFFSET(::sayo::ASRResult, _internal_metadata_),
        ~0u,  // no _extensions_
        ~0u,  // no _oneof_case_
        ~0u,  // no _weak_field_map_
        ~0u,  // no _inlined_string_donated_
        ~0u,  // no _split_
        ~0u,  // no sizeof(Split)
        PROTOBUF_FIELD_OFFSET(::sayo::ASRResult, _impl_.text_),
        ~0u,  // no _has_bits_
        PROTOBUF_FIELD_OFFSET(::sayo::PingRequest, _internal_metadata_),
        ~0u,  // no _extensions_
        ~0u,  // no _oneof_case_
        ~0u,  // no _weak_field_map_
        ~0u,  // no _inlined_string_donated_
        ~0u,  // no _split_
        ~0u,  // no sizeof(Split)
        ~0u,  // no _has_bits_
        PROTOBUF_FIELD_OFFSET(::sayo::PingResponse, _internal_metadata_),
        ~0u,  // no _extensions_
        ~0u,  // no _oneof_case_
        ~0u,  // no _weak_field_map_
        ~0u,  // no _inlined_string_donated_
        ~0u,  // no _split_
        ~0u,  // no sizeof(Split)
        PROTOBUF_FIELD_OFFSET(::sayo::PingResponse, _impl_.message_),
};

static const ::_pbi::MigrationSchema
    schemas[] ABSL_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
        {0, -1, -1, sizeof(::sayo::AudioChunk)},
        {9, -1, -1, sizeof(::sayo::ASRResult)},
        {18, -1, -1, sizeof(::sayo::PingRequest)},
        {26, -1, -1, sizeof(::sayo::PingResponse)},
};
static const ::_pb::Message* const file_default_instances[] = {
    &::sayo::_AudioChunk_default_instance_._instance,
    &::sayo::_ASRResult_default_instance_._instance,
    &::sayo::_PingRequest_default_instance_._instance,
    &::sayo::_PingResponse_default_instance_._instance,
};
const char descriptor_table_protodef_sayo_2eproto[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
    protodesc_cold) = {
    "\n\nsayo.proto\022\004sayo\"\031\n\nAudioChunk\022\013\n\003pcm\030"
    "\001 \001(\014\"\031\n\tASRResult\022\014\n\004text\030\001 \001(\t\"\r\n\013Ping"
    "Request\"\037\n\014PingResponse\022\017\n\007message\030\001 \001(\t"
    "2s\n\013SayoService\0225\n\014StreamingASR\022\020.sayo.A"
    "udioChunk\032\017.sayo.ASRResult(\0010\001\022-\n\004Ping\022\021"
    ".sayo.PingRequest\032\022.sayo.PingResponseb\006p"
    "roto3"
};
static ::absl::once_flag descriptor_table_sayo_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_sayo_2eproto = {
    false,
    false,
    245,
    descriptor_table_protodef_sayo_2eproto,
    "sayo.proto",
    &descriptor_table_sayo_2eproto_once,
    nullptr,
    0,
    4,
    schemas,
    file_default_instances,
    TableStruct_sayo_2eproto::offsets,
    file_level_metadata_sayo_2eproto,
    file_level_enum_descriptors_sayo_2eproto,
    file_level_service_descriptors_sayo_2eproto,
};

// This function exists to be marked as weak.
// It can significantly speed up compilation by breaking up LLVM's SCC
// in the .pb.cc translation units. Large translation units see a
// reduction of more than 35% of walltime for optimized builds. Without
// the weak attribute all the messages in the file, including all the
// vtables and everything they use become part of the same SCC through
// a cycle like:
// GetMetadata -> descriptor table -> default instances ->
//   vtables -> GetMetadata
// By adding a weak function here we break the connection from the
// individual vtables back into the descriptor table.
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_sayo_2eproto_getter() {
  return &descriptor_table_sayo_2eproto;
}
namespace sayo {
// ===================================================================

class AudioChunk::_Internal {
 public:
};

AudioChunk::AudioChunk(::google::protobuf::Arena* arena)
    : ::google::protobuf::Message(arena) {
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:sayo.AudioChunk)
}
inline PROTOBUF_NDEBUG_INLINE AudioChunk::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility, ::google::protobuf::Arena* arena,
    const Impl_& from)
      : pcm_(arena, from.pcm_),
        _cached_size_{0} {}

AudioChunk::AudioChunk(
    ::google::protobuf::Arena* arena,
    const AudioChunk& from)
    : ::google::protobuf::Message(arena) {
  AudioChunk* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_);

  // @@protoc_insertion_point(copy_constructor:sayo.AudioChunk)
}
inline PROTOBUF_NDEBUG_INLINE AudioChunk::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : pcm_(arena),
        _cached_size_{0} {}

inline void AudioChunk::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
}
AudioChunk::~AudioChunk() {
  // @@protoc_insertion_point(destructor:sayo.AudioChunk)
  _internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  SharedDtor();
}
inline void AudioChunk::SharedDtor() {
  ABSL_DCHECK(GetArena() == nullptr);
  _impl_.pcm_.Destroy();
  _impl_.~Impl_();
}

const ::google::protobuf::MessageLite::ClassData*
AudioChunk::GetClassData() const {
  PROTOBUF_CONSTINIT static const ::google::protobuf::MessageLite::
      ClassDataFull _data_ = {
          {
              nullptr,  // OnDemandRegisterArenaDtor
              PROTOBUF_FIELD_OFFSET(AudioChunk, _impl_._cached_size_),
              false,
          },
          &AudioChunk::MergeImpl,
          &AudioChunk::kDescriptorMethods,
      };
  return &_data_;
}
PROTOBUF_NOINLINE void AudioChunk::Clear() {
// @@protoc_insertion_point(message_clear_start:sayo.AudioChunk)
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.pcm_.ClearToEmpty();
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

const char* AudioChunk::_InternalParse(
    const char* ptr, ::_pbi::ParseContext* ctx) {
  ptr = ::_pbi::TcParser::ParseLoop(this, ptr, ctx, &_table_.header);
  return ptr;
}


PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<0, 1, 0, 0, 2> AudioChunk::_table_ = {
  {
    0,  // no _has_bits_
    0, // no _extensions_
    1, 0,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967294,  // skipmap
    offsetof(decltype(_table_), field_entries),
    1,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    &_AudioChunk_default_instance_._instance,
    ::_pbi::TcParser::GenericFallback,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::sayo::AudioChunk>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    // bytes pcm = 1;
    {::_pbi::TcParser::FastBS1,
     {10, 63, 0, PROTOBUF_FIELD_OFFSET(AudioChunk, _impl_.pcm_)}},
  }}, {{
    65535, 65535
  }}, {{
    // bytes pcm = 1;
    {PROTOBUF_FIELD_OFFSET(AudioChunk, _impl_.pcm_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kBytes | ::_fl::kRepAString)},
  }},
  // no aux_entries
  {{
  }},
};

::uint8_t* AudioChunk::_InternalSerialize(
    ::uint8_t* target,
    ::google::protobuf::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:sayo.AudioChunk)
  ::uint32_t cached_has_bits = 0;
  (void)cached_has_bits;

  // bytes pcm = 1;
  if (!this->_internal_pcm().empty()) {
    const std::string& _s = this->_internal_pcm();
    target = stream->WriteBytesMaybeAliased(1, _s, target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target =
        ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
            _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:sayo.AudioChunk)
  return target;
}

::size_t AudioChunk::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:sayo.AudioChunk)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // bytes pcm = 1;
  if (!this->_internal_pcm().empty()) {
    total_size += 1 + ::google::protobuf::internal::WireFormatLite::BytesSize(
                                    this->_internal_pcm());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}


void AudioChunk::MergeImpl(::google::protobuf::MessageLite& to_msg, const ::google::protobuf::MessageLite& from_msg) {
  auto* const _this = static_cast<AudioChunk*>(&to_msg);
  auto& from = static_cast<const AudioChunk&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:sayo.AudioChunk)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_pcm().empty()) {
    _this->_internal_set_pcm(from._internal_pcm());
  }
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void AudioChunk::CopyFrom(const AudioChunk& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:sayo.AudioChunk)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

PROTOBUF_NOINLINE bool AudioChunk::IsInitialized() const {
  return true;
}

void AudioChunk::InternalSwap(AudioChunk* PROTOBUF_RESTRICT other) {
  using std::swap;
  auto* arena = GetArena();
  ABSL_DCHECK_EQ(arena, other->GetArena());
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.pcm_, &other->_impl_.pcm_, arena);
}

::google::protobuf::Metadata AudioChunk::GetMetadata() const {
  return ::_pbi::AssignDescriptors(&descriptor_table_sayo_2eproto_getter,
                                   &descriptor_table_sayo_2eproto_once,
                                   file_level_metadata_sayo_2eproto[0]);
}
// ===================================================================

class ASRResult::_Internal {
 public:
};

ASRResult::ASRResult(::google::protobuf::Arena* arena)
    : ::google::protobuf::Message(arena) {
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:sayo.ASRResult)
}
inline PROTOBUF_NDEBUG_INLINE ASRResult::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility, ::google::protobuf::Arena* arena,
    const Impl_& from)
      : text_(arena, from.text_),
        _cached_size_{0} {}

ASRResult::ASRResult(
    ::google::protobuf::Arena* arena,
    const ASRResult& from)
    : ::google::protobuf::Message(arena) {
  ASRResult* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_);

  // @@protoc_insertion_point(copy_constructor:sayo.ASRResult)
}
inline PROTOBUF_NDEBUG_INLINE ASRResult::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : text_(arena),
        _cached_size_{0} {}

inline void ASRResult::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
}
ASRResult::~ASRResult() {
  // @@protoc_insertion_point(destructor:sayo.ASRResult)
  _internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  SharedDtor();
}
inline void ASRResult::SharedDtor() {
  ABSL_DCHECK(GetArena() == nullptr);
  _impl_.text_.Destroy();
  _impl_.~Impl_();
}

const ::google::protobuf::MessageLite::ClassData*
ASRResult::GetClassData() const {
  PROTOBUF_CONSTINIT static const ::google::protobuf::MessageLite::
      ClassDataFull _data_ = {
          {
              nullptr,  // OnDemandRegisterArenaDtor
              PROTOBUF_FIELD_OFFSET(ASRResult, _impl_._cached_size_),
              false,
          },
          &ASRResult::MergeImpl,
          &ASRResult::kDescriptorMethods,
      };
  return &_data_;
}
PROTOBUF_NOINLINE void ASRResult::Clear() {
// @@protoc_insertion_point(message_clear_start:sayo.ASRResult)
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.text_.ClearToEmpty();
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

const char* ASRResult::_InternalParse(
    const char* ptr, ::_pbi::ParseContext* ctx) {
  ptr = ::_pbi::TcParser::ParseLoop(this, ptr, ctx, &_table_.header);
  return ptr;
}


PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<0, 1, 0, 27, 2> ASRResult::_table_ = {
  {
    0,  // no _has_bits_
    0, // no _extensions_
    1, 0,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967294,  // skipmap
    offsetof(decltype(_table_), field_entries),
    1,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    &_ASRResult_default_instance_._instance,
    ::_pbi::TcParser::GenericFallback,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::sayo::ASRResult>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    // string text = 1;
    {::_pbi::TcParser::FastUS1,
     {10, 63, 0, PROTOBUF_FIELD_OFFSET(ASRResult, _impl_.text_)}},
  }}, {{
    65535, 65535
  }}, {{
    // string text = 1;
    {PROTOBUF_FIELD_OFFSET(ASRResult, _impl_.text_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kUtf8String | ::_fl::kRepAString)},
  }},
  // no aux_entries
  {{
    "\16\4\0\0\0\0\0\0"
    "sayo.ASRResult"
    "text"
  }},
};

::uint8_t* ASRResult::_InternalSerialize(
    ::uint8_t* target,
    ::google::protobuf::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:sayo.ASRResult)
  ::uint32_t cached_has_bits = 0;
  (void)cached_has_bits;

  // string text = 1;
  if (!this->_internal_text().empty()) {
    const std::string& _s = this->_internal_text();
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
        _s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormatLite::SERIALIZE, "sayo.ASRResult.text");
    target = stream->WriteStringMaybeAliased(1, _s, target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target =
        ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
            _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:sayo.ASRResult)
  return target;
}

::size_t ASRResult::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:sayo.ASRResult)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string text = 1;
  if (!this->_internal_text().empty()) {
    total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                    this->_internal_text());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}


void ASRResult::MergeImpl(::google::protobuf::MessageLite& to_msg, const ::google::protobuf::MessageLite& from_msg) {
  auto* const _this = static_cast<ASRResult*>(&to_msg);
  auto& from = static_cast<const ASRResult&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:sayo.ASRResult)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_text().empty()) {
    _this->_internal_set_text(from._internal_text());
  }
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void ASRResult::CopyFrom(const ASRResult& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:sayo.ASRResult)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

PROTOBUF_NOINLINE bool ASRResult::IsInitialized() const {
  return true;
}

void ASRResult::InternalSwap(ASRResult* PROTOBUF_RESTRICT other) {
  using std::swap;
  auto* arena = GetArena();
  ABSL_DCHECK_EQ(arena, other->GetArena());
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.text_, &other->_impl_.text_, arena);
}

::google::protobuf::Metadata ASRResult::GetMetadata() const {
  return ::_pbi::AssignDescriptors(&descriptor_table_sayo_2eproto_getter,
                                   &descriptor_table_sayo_2eproto_once,
                                   file_level_metadata_sayo_2eproto[1]);
}
// ===================================================================

class PingRequest::_Internal {
 public:
};

PingRequest::PingRequest(::google::protobuf::Arena* arena)
    : ::google::protobuf::internal::ZeroFieldsBase(arena) {
  // @@protoc_insertion_point(arena_constructor:sayo.PingRequest)
}
PingRequest::PingRequest(
    ::google::protobuf::Arena* arena,
    const PingRequest& from)
    : ::google::protobuf::internal::ZeroFieldsBase(arena) {
  PingRequest* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);

  // @@protoc_insertion_point(copy_constructor:sayo.PingRequest)
}









::google::protobuf::Metadata PingRequest::GetMetadata() const {
  return ::_pbi::AssignDescriptors(&descriptor_table_sayo_2eproto_getter,
                                   &descriptor_table_sayo_2eproto_once,
                                   file_level_metadata_sayo_2eproto[2]);
}
// ===================================================================

class PingResponse::_Internal {
 public:
};

PingResponse::PingResponse(::google::protobuf::Arena* arena)
    : ::google::protobuf::Message(arena) {
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:sayo.PingResponse)
}
inline PROTOBUF_NDEBUG_INLINE PingResponse::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility, ::google::protobuf::Arena* arena,
    const Impl_& from)
      : message_(arena, from.message_),
        _cached_size_{0} {}

PingResponse::PingResponse(
    ::google::protobuf::Arena* arena,
    const PingResponse& from)
    : ::google::protobuf::Message(arena) {
  PingResponse* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_);

  // @@protoc_insertion_point(copy_constructor:sayo.PingResponse)
}
inline PROTOBUF_NDEBUG_INLINE PingResponse::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : message_(arena),
        _cached_size_{0} {}

inline void PingResponse::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
}
PingResponse::~PingResponse() {
  // @@protoc_insertion_point(destructor:sayo.PingResponse)
  _internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  SharedDtor();
}
inline void PingResponse::SharedDtor() {
  ABSL_DCHECK(GetArena() == nullptr);
  _impl_.message_.Destroy();
  _impl_.~Impl_();
}

const ::google::protobuf::MessageLite::ClassData*
PingResponse::GetClassData() const {
  PROTOBUF_CONSTINIT static const ::google::protobuf::MessageLite::
      ClassDataFull _data_ = {
          {
              nullptr,  // OnDemandRegisterArenaDtor
              PROTOBUF_FIELD_OFFSET(PingResponse, _impl_._cached_size_),
              false,
          },
          &PingResponse::MergeImpl,
          &PingResponse::kDescriptorMethods,
      };
  return &_data_;
}
PROTOBUF_NOINLINE void PingResponse::Clear() {
// @@protoc_insertion_point(message_clear_start:sayo.PingResponse)
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.message_.ClearToEmpty();
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

const char* PingResponse::_InternalParse(
    const char* ptr, ::_pbi::ParseContext* ctx) {
  ptr = ::_pbi::TcParser::ParseLoop(this, ptr, ctx, &_table_.header);
  return ptr;
}


PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<0, 1, 0, 33, 2> PingResponse::_table_ = {
  {
    0,  // no _has_bits_
    0, // no _extensions_
    1, 0,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967294,  // skipmap
    offsetof(decltype(_table_), field_entries),
    1,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    &_PingResponse_default_instance_._instance,
    ::_pbi::TcParser::GenericFallback,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::sayo::PingResponse>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    // string message = 1;
    {::_pbi::TcParser::FastUS1,
     {10, 63, 0, PROTOBUF_FIELD_OFFSET(PingResponse, _impl_.message_)}},
  }}, {{
    65535, 65535
  }}, {{
    // string message = 1;
    {PROTOBUF_FIELD_OFFSET(PingResponse, _impl_.message_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kUtf8String | ::_fl::kRepAString)},
  }},
  // no aux_entries
  {{
    "\21\7\0\0\0\0\0\0"
    "sayo.PingResponse"
    "message"
  }},
};

::uint8_t* PingResponse::_InternalSerialize(
    ::uint8_t* target,
    ::google::protobuf::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:sayo.PingResponse)
  ::uint32_t cached_has_bits = 0;
  (void)cached_has_bits;

  // string message = 1;
  if (!this->_internal_message().empty()) {
    const std::string& _s = this->_internal_message();
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
        _s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormatLite::SERIALIZE, "sayo.PingResponse.message");
    target = stream->WriteStringMaybeAliased(1, _s, target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target =
        ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
            _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:sayo.PingResponse)
  return target;
}

::size_t PingResponse::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:sayo.PingResponse)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string message = 1;
  if (!this->_internal_message().empty()) {
    total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                    this->_internal_message());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}


void PingResponse::MergeImpl(::google::protobuf::MessageLite& to_msg, const ::google::protobuf::MessageLite& from_msg) {
  auto* const _this = static_cast<PingResponse*>(&to_msg);
  auto& from = static_cast<const PingResponse&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:sayo.PingResponse)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_message().empty()) {
    _this->_internal_set_message(from._internal_message());
  }
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void PingResponse::CopyFrom(const PingResponse& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:sayo.PingResponse)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

PROTOBUF_NOINLINE bool PingResponse::IsInitialized() const {
  return true;
}

void PingResponse::InternalSwap(PingResponse* PROTOBUF_RESTRICT other) {
  using std::swap;
  auto* arena = GetArena();
  ABSL_DCHECK_EQ(arena, other->GetArena());
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.message_, &other->_impl_.message_, arena);
}

::google::protobuf::Metadata PingResponse::GetMetadata() const {
  return ::_pbi::AssignDescriptors(&descriptor_table_sayo_2eproto_getter,
                                   &descriptor_table_sayo_2eproto_once,
                                   file_level_metadata_sayo_2eproto[3]);
}
// @@protoc_insertion_point(namespace_scope)
}  // namespace sayo
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google
// @@protoc_insertion_point(global_scope)
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2
static ::std::false_type _static_init_ PROTOBUF_UNUSED =
    (::_pbi::AddDescriptors(&descriptor_table_sayo_2eproto),
     ::std::false_type{});
#include "google/protobuf/port_undef.inc"
