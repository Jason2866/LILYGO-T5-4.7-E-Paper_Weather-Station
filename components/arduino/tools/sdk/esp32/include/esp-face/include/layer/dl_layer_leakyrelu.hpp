#pragma once

#include "dl_constant.hpp"
#include "dl_variable.hpp"
#include "dl_nn_LeakyReLU.hpp"
#include "dl_layer_base.hpp"

namespace dl
{
    namespace layer
    {
        /**
         * @brief LeakyReLU(input).
         * 
         * @tparam feature_t supports int16_t and int8_t,
         *         - int16_t: stands for operation in int16_t quantize
         *         - int8_t: stands for operation in int8_t quantize
         */
        template <typename feature_t>
        class LeakyReLU : public Layer
        {
        private:
            feature_t activation_alpha; /*<! quantized alpha >*/
            int activation_exponent;    /*<! exponent of quantized alpha >*/
            Tensor<feature_t> *output;  /*<! output ptr of leakyrelu>*/
            bool inplace;               /*<! true: the output will store to input0
                                             false: the output will store to a seperate memeory >*/
        public:

            /**
             * @brief Construct a new LeakyReLU object
             * 
             * @param activation_alpha     quantized alpha
             * @param activation_exponent  exponent of quantized alpha
             * @param name                 name of leakyrelu
             * @param inplace              true: the output will store to input0
             *                             false: the output will store to a seperate memeory
             */
            LeakyReLU(const int activation_alpha, const int activation_exponent, const char *name = NULL, bool inplace = false) : Layer(name)
            {
                this->activation_alpha = activation_alpha;
                this->activation_exponent = activation_exponent;
                this->inplace = inplace;
            }

            /**
             * @brief Destroy the LeakyReLU object
             * 
             */
            ~LeakyReLU() 
            {
                if ((!this->inplace) && (this->output != NULL))
                {
                    delete this->output;
                }
            }

            /**
             * @brief Update output shape and exponent
             * 
             * @param input       as an input
             */
            void build(Tensor<feature_t> &input)
            {
                if(!this->inplace)
                {
                    if(this->output != NULL)
                    {
                        this->output = new Tensor<feature_t>;
                    }  
                    this->output->set_shape(input.shape);
                    this->output->set_exponent(input.exponent);
                    this->output->free_element();
                }
                else
                {
                    this->output = &input;
                }
                
            }

            /**
             * @brief Get the output 
             * 
             * @return Tensor<feature_t>& LeakyReLU result
             */
            Tensor<feature_t> &get_output()
            {
                return *this->output;
            }

            /**
             * @brief Call LeakyReLU operation.
             * 
             * @param input       as an input
             * @param assign_core not effective yet
             * @return LeakyReLU result
             */
            Tensor<feature_t> &call(Tensor<feature_t> &input, const std::vector<int> &assign_core = CONFIG_DEFAULT_ASSIGN_CORE)
            {
                DL_LOG_LAYER_LATENCY_INIT();

                if(!this->inplace)
                {
                    DL_LOG_LAYER_LATENCY_START();
                    this->output->apply_element();
                    this->output->set_exponent(input.exponent);
                    DL_LOG_LAYER_LATENCY_END(this->name, "apply");

                    DL_LOG_LAYER_LATENCY_START();
                    nn::leakyrelu(*this->output, input, this->activation_alpha, this->activation_exponent, assign_core);
                    DL_LOG_LAYER_LATENCY_END(this->name, "leakyrelu");
                }
                else
                {
                    DL_LOG_LAYER_LATENCY_START();
                    nn::leakyrelu<true>(*this->output, input, this->activation_alpha, this->activation_exponent, assign_core);
                    DL_LOG_LAYER_LATENCY_END(this->name, "leakyrelu");
                }

                return *this->output;
            }
        };
    } // namespace layer
} // namespace dl
