#ifndef RENDERER_BASE_PARAMETERS_H
#define RENDERER_BASE_PARAMETERS_H

namespace Module::Renderer {

    enum class FrequencyScale : unsigned int {
        Linear      = 0,
        Logarithmic = 1,
        Mel         = 2,
    };

    class Parameters {
#define DEFINE_PROPERTY(type, field, getter, setter)        \
    public:                                                 \
        const type& getter () const { return m_##field; }   \
        Parameters *setter (const type& v) {                \
            m_##field = v; return this;                     \
        }                                                   \
    private:                                                \
        type m_##field;

    DEFINE_PROPERTY(double,          minFreq,        getMinFrequency,    setMinFrequency)
    DEFINE_PROPERTY(double,          maxFreq,        getMaxFrequency,    setMaxFrequency)
    DEFINE_PROPERTY(double,          minGain,        getMinGain,         setMinGain)
    DEFINE_PROPERTY(double,          maxGain,        getMaxGain,         setMaxGain)
    DEFINE_PROPERTY(FrequencyScale, frequencyScale, getFrequencyScale,  setFrequencyScale)

#undef DEFINE_PROPERTY
    };

}

#endif // RENDERER_BASE_PARAMETERS_H
