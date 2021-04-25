/*!
 * project = s : "landb"
 *
 *
 * (credits:
 *          message = s : "Created by René Descartes Domingos Muala on 10/10/20."
 *          Copyright = s : "© 2021 landia (René Muala). All rights reserved."
 *          Contact = s : "renemuala@icloud.com"
 * )
 */

#pragma once

#include <iostream>
#include <string>

namespace lan
{
    /* lan::safe_file */
    class safe_file {
        FILE * file;
        std::string filename;

    public:
        
        safe_file();
        
        /* opens a file */
        bool open(std::string);
        /* checks if the file is opened */
        bool check();
        /* pushes a string into the current file */
        bool push(std::string);
        /* pulls a string from the current file */
        std::string pull();
        /* gets the length of the current file */
        size_t length();
        /* closes the current file */
        bool close();
        /* closes the current file descriptor*/
        bool close_fd();

        ~safe_file();
    };
    
    const std::string safe_file_version = "1.0 (stable)";
    
    /* lan::db */
    
    const std::string db_version = "2.6 (public)";
    
    namespace errors {
        //! @brief catch parameter to handle errors with bit names
        typedef std::invalid_argument bit_name_error;
        typedef std::out_of_range anchor_name_error;
        typedef std::out_of_range empty_anchor_error;
        typedef std::logic_error pull_error;
        typedef std::runtime_error overriding_bit_error;
        
        namespace _private {
            enum error_type {_bit_name_error, _anchor_name_error, _empty_anchor_error, _overriding_bit_error};
        }
    }
    
    //! @brief database bit type
    enum db_bit_type {Bool , Int, Long, LongLong, Float, Double, Char, String, Unsafe, Array, Container};
    
    /*
     *     namespace _private {
     *        char db_bit_table [11] = {  'b' ,   'i' ,
     *                                    'l' ,   'x' ,
     *                                    'f' ,   'd' ,
     *                                    'c' ,   's' ,
     *                                    'u' ,   'a' ,
     *                                            '#' };
     *      }
     *
     */
    
    //! @brief database bit: used to criate linked lists that store variables, arrays and containers dynamically
    struct db_bit {
        std::string     key;
        db_bit_type     type;
        void *          data;
        struct db_bit * pre, * nex, * lin, * con;
        db_bit(){
            key.clear();
            data = nullptr;
            pre  = nullptr;
            nex  = nullptr;
            lin  = nullptr;
            con  = nullptr; 
        } ~ db_bit (){
            if(data or not key.empty()) { ::free(data) ; db_bit();}
            if(lin) {delete lin; lin = nullptr;}
        }
    };
    
    typedef db_bit db_bits;
    typedef db_bit anchor_t;
    
    /// @brief Landia Database
    class db {
        
        lan::db_bits * data;
        lan::db_bit  * first, * last;
        lan::anchor_t * anchor;
        lan::safe_file file;
        
    public:
        
        db();
        
        /* -- */
        
        /* Erases all bits in the context. */
        void erase_bits(db_bits *);
        
        /* Erases a bit. */
        void erase_bit(db_bit *);
        
        /* Resets pointers and variables of the class. */
        void reset_data();
        
        /* Erases current loaded bits and frees memory. */
        void erase();
        
        /* The database is empty. */
        bool empty();
        
        /*! Prints the bits of the current context (default context: main from *first).
         *  @param tabs Used by the system.
         *  @param  bit The bit to start printing from.
         */
        void print(size_t tabs = 0 , lan::db_bit * bit = nullptr);
        
        /* File */
        
        /* Connects the database to a file, to pull and push from. */
        bool connect(std::string);
        
        /* Database is connected to a file. */
        bool is_connected();
        
        /* Disconnects the database from the current file. */
        bool disconnect();
        
        /*! @brief Pull dependece. */
        size_t  str_get_str_end(std::string, size_t = 0);
        
        /*! @brief Pull dependece. */
        ssize_t str_find_first_of(std::string const , std::string const, size_t = 0);
        
        /*! @brief Pull dependece. */
        ssize_t str_find_first_not_of(std::string const, std::string const, size_t = 0);
        
        /*! @brief Pull dependece. */
        bool str_compare(const char , const std::string );
        
        /*! @brief Pull dependece. */
        std::string pop_next(std::string &);
        
        /*! @brief Pull dependece. */
        lan::db_bit * get_container_data(std::string &);
        
        /*! @brief Pull dependece. */
        lan::db_bit  * read_container_bit(std::string &, bool = false);
        
        /*! @brief Pull dependece. */
        lan::db_bit * get_array_data(std::string &);
        
        /*! @brief Pull dependece. */
        lan::db_bit  * read_array_bit(std::string const, std::string &);
        
        /*! @brief Pull dependece. */
        lan::db_bit_type convert_to_bit_type(char);
        
        /*! @brief Pull dependece. */
        std::string prepare_string_to_read(std::string);
        
        /*! @brief Pull dependece. */
        void * get_var_data(db_bit_type, std::string);
        
        /*! @brief Pull dependece. */
        lan::db_bit  * read_var_bit(std::string const, db_bit_type const, std::string &);
        
        /*! @brief Pull dependece. */
        lan::db_bit  * read_value_bit(std::string &, bool = false);
        
        /*! @brief Pull dependece. */
        lan::db_bit  * read_bit(std::string &);
        
        /*! @brief Pull dependece. */
        lan::db_bits * read_all_bits(std::string);
        
        /*! @brief Pulls data from the connected file. Note: This operaion erases all bits */
        bool pull();
        
        /*! @brief Push dependece.*/
        std::string write_container_bit(db_bit *);
        
        /*! @brief Push dependece.*/
        std::string write_array_bit(db_bit *);
        
        /*! @brief Push dependece.*/
        std::string prepare_char_to_write(char);
        
        /*! @brief Push dependece.*/
        std::string prepare_string_to_write(std::string);
        
        /*! @brief Push dependece.*/
        std::string write_var_bit(db_bit *, bool = false);
        
        /*! @brief Push dependece.*/
        std::string write_bit(db_bit *, bool = false);
        
        /*! @brief Push dependece.*/
        std::string write_all_bits(db_bits *);
        
        /*! @brief Pushes data to the current file, in landb-structure.*/
        bool push();
        
        /* Error handling */
        
        /*! @brief General dependece */
        std::string error_string(errors::_private::error_type, std::string const);
        
        /* db general */
        
        /*! @brief Updates the *last pointer */
        bool update_last(){return (last = (first and last) ? last : get_last_bit(first));}
        
        /*! @brief Sets a variable bit, depence.
         @param context The context of the variable.
         @param var     The bit that will be set.
         @param name    The name of the bit.
         @param type    The type of the bit.
         @param value   The literal value of the bit.
         */
        template<typename any>
        bool set_bit(db_bit * context, db_bit * var,std::string const name, db_bit_type const type, any const value){
            set_bit(context, var, name, type);
            var->data = new any (value);
            return (var->data);
        }
        
        /*! @brief Sets a bit, dependece.
         @param context The context of the variable.
         @param var     The bit that will be set.
         @param name    The name of the bit.
         @param type    The type of the bit.
         */
        bool set_bit(db_bit * context, db_bit * var,std::string const name, db_bit_type const type){
            var->~db_bit();
            var->key  = name;
            var->type = type;
            var->con = context;
            return  (var);
        }
        
        /*! @brief Inits the *first bit, dependece.
         @param name    The name of the bit.
         @param value   The literal value of the bit.
         @param type    The type of the bit.
         */
        template<typename any>
        bool init(std::string const name, any const value, db_bit_type const type){
            data = new db_bit;
            return (first = last = data) and ((type < lan::Array) ? set_bit(nullptr, data, name, type, value) : set_bit(nullptr ,data, name, type));
        }
        
        /*! @brief Inits the *first bit of a context, dependece.
         @param context The context.
         @param name    The name of the bit.
         @param value   The literal value of the bit.
         @param type    The type of the bit.
         */
        template<typename any>
        bool init(lan::db_bit * context, std::string const name, any const value, db_bit_type const type){
            data = context;
            data->lin = new db_bit;
            return ((type < lan::Array) ? set_bit(data, data->lin, name, type, value) : set_bit(data, data->lin, name, type));
        }
        
        /*! @brief Appends a bit in the main context, dependece.
         @param name    The name of the bit.
         @param value   The literal value of the bit.
         @param type    The type of the bit.
         */
        template<typename any>
        bool append(std::string const name, any const value, db_bit_type const type){
            last = get_last_bit(first);
            last->nex = new db_bit; last->nex->pre = last;
            return (last=last->nex) and ((type < lan::Array) ? set_bit(nullptr, last, name, type, value) : set_bit(nullptr, last, name, type));
        }
        
        /*! @brief Appends a bit in a context, dependece.
         @param context The context.
         @param name    The name of the bit.
         @param value   The literal value of the bit.
         @param type    The type of the bit.
         */
        template<typename any>
        bool append(lan::db_bit * context, std::string const name, any const value, db_bit_type const type){
            data = context;
            if(data-> type == lan::Container and (data = data->lin)){
                data = get_last_bit(data);
                data->nex = new db_bit; data->nex->pre = data; data = data->nex;
                return ((type < lan::Array) ? set_bit(context, data, name, type, value) : set_bit(context, data, name, type));
            } return false;
        }
        
        /*! @brief Inits a bit in a context, dependece.
         @param context The context.
         @param name    The name of the bit.
         @param value   The literal value of the bit.
         @param type    The type of the bit.
         */
        template<typename any>
        bool init_iter(lan::db_bit * context, std::string const name, any const value, db_bit_type const type){
            data = context;
            data->lin = new db_bit; data = data->lin;
            return ((type < lan::Array) ? set_bit(context, data, name, type, value) : set_bit(context, data, name, type));
        }
        
        /*! @brief Appends a bit in a context, dependece.
         @param context The context.
         @param name    The name of the bit.
         @param value   The literal value of the bit.
         @param type    The ype of the bit.
         */
        template<typename any>
        bool append_iter(lan::db_bit * context, std::string const name, any const value, db_bit_type const type){
            data = context;
            if(context->type == lan::Array and (data = data->lin)){
                data = get_last_bit(data);
                data->nex = new db_bit; data->nex->pre = data; data = data->nex;
                return ((type < lan::Array) ? set_bit(context, data, name, type, value) : set_bit(context, data, name, type));
            } return false;
        }
        
        /*! @brief Appends a bit in an array.
         @param target  The array.
         @param value   The literal value of the bit.
         @param type    The type of the bit.
         */
        template<typename any>
        bool iterate(std::string const target, any const value, db_bit_type const type){
            if((data = find_rec(target, lan::Container, lan::Array, first))) {
                return (data->lin) ? append_iter(data, "", value, type) :
                init_iter(data, "", value, type);
            } throw lan::errors::bit_name_error(error_string(errors::_private::_bit_name_error, target+"{a}"));
        }
        
        /*! @brief Declares an array or container bit in the main context.
         @param name    The name of the bit.
         @param type    The type of the bit.
         */
        bool declare(std::string const name, db_bit_type const type);
        
        /*! @brief Declares an array or container bit in a certain context.
         @param name    The name of the bit.
         @param type    The type of the bit.
         */
        bool declare(std::string const target, std::string const name, db_bit_type const type);
        
        /*! @brief Global dependece. */
        std::string find__pop_address(std::string &);
        
        /*! @brief Global dependece. */
        lan::db_bit * find(std::string const, lan::db_bit *);
        
        /*! @brief Global dependece. */
        lan::db_bit * find_rec(std::string, lan::db_bit_type const , lan::db_bit *);
        
        /*! @brief Global dependece. */
        lan::db_bit * find_rec(std::string, lan::db_bit_type const , lan::db_bit_type const, lan::db_bit *);
        
        /*! @brief Global dependece. */
        lan::db_bit * find_var(std::string const, lan::db_bit *);
        
        /*! @brief Global dependece. */
        lan::db_bit * find_non_var(std::string const, lan::db_bit *);
        
        /*! @brief Global dependece. */
        lan::db_bit * find_any(std::string const, lan::db_bit_type const, lan::db_bit *);
        
        /* Get */
        
        /*! @brief Gets data from a variable bit in the main context.
         @param name    The name of the bit.
         @param type    The type of the bit.
         Eg: any.get<int>(...);
         */
        template<typename any>
        any get(std::string const name, const lan::db_bit_type type){
            if((data = find_any(name, type, first)) and data->data and type < lan::Array){
                any * data_p = (any*)data->data;
                return ((any&)*data_p);
            } throw lan::errors::bit_name_error(error_string(errors::_private::_bit_name_error, name));
        }
        
        /*! @brief Gets *data from a variable bit in the main context.
         @param name    The name of the bit.
         @param type    The type of the bit.
         Eg: int * p = any.get_p<int>(...);
         */
        template<typename any>
        any * get_p(std::string const name, const lan::db_bit_type type){
            if((data = find_any(name, type, first)) and data->data and type < lan::Array){
                return (any*)data->data;
            } throw lan::errors::bit_name_error(error_string(errors::_private::_bit_name_error, name));
        }
        
        /*! @brief Gets data from a variable bit from an array in the main context.
         @param name    The name of the bit.
         @param index   The index of the bit.
         @param type    The type of the bit.
         Eg: any.get<int>(...);
         */
        template<typename any>
        any get(std::string const name, size_t index, const lan::db_bit_type type){
            if((data = find_any(name, lan::Array, first))){
                data=data->lin;
                for(register_t it = 0 ; (it < index) and data ; it++, data=data->nex);
                if(data and data->type == type){
                    any * data_p = (any*)data->data;
                    return ((any&)*data_p);
                } throw lan::errors::bit_name_error(error_string(errors::_private::_bit_name_error, name+"["+std::to_string(index)+"]"));
            } throw lan::errors::bit_name_error(error_string(errors::_private::_bit_name_error, name));
        }
        
        /*! @brief gets *data from an array in the main context.
         @param name    The name of the bit.
         @param index   The index of the bit.
         @param type    The type of the bit.
         Eg: int * p = any.get_p<int>(...);
         */
        template<typename any>
        any * get_p(std::string const name, size_t index, const lan::db_bit_type type){
            if((data = find_any(name, lan::Array, first))){
                data=data->lin;
                for(register_t it = 0 ; (it < index) and data ; it++, data=data->nex);
                if(data and data->type == type){
                    return (any*)data->data;
                } throw lan::errors::bit_name_error(error_string(errors::_private::_bit_name_error, name+"["+std::to_string(index)+"]"));
            } throw lan::errors::bit_name_error(error_string(errors::_private::_bit_name_error, name));
        }
        
        /*! @brief Gets data from a variable bit in a certain context.
         @param context The context.
         @param name    The name of the bit.
         @param type    The type of the bit.
         Eg: any.get<int>(...);
         */
        template<typename any>
        any get(std::string context, std::string const name, const lan::db_bit_type type){
            if ((data = find_rec(context, lan::Container, first))) {
                if((data = find_any(name, type, data->lin)) and data->data){
                    any * data_p = (any*)data->data;
                    return ((any&)*data_p);
                }
            } throw lan::errors::bit_name_error(error_string(errors::_private::_bit_name_error, name));
        }
        
        /*! @brief Gets *data from a variable bit in a certain context.
         @param context The context.
         @param name    The name of the bit.
         @param type    The type of the bit.
         Eg: int * p = any.get_p<int>(...);
         */
        template<typename any>
        any * get_p(std::string context, std::string const name, const lan::db_bit_type type){
            if ((data = find_rec(context, lan::Container, first))) {
                if((data = find_any(name, type, data->lin)) and data->data){
                    return (any*)data->data;
                }
            } throw lan::errors::bit_name_error(error_string(errors::_private::_bit_name_error, name));
        }
        
        
        /*! @brief Gets data from a variable bit.
         @param bit   The bit.
         Eg: any.get<int>(...);
         */
        template<typename any>
        any get(db_bit * bit){
            if(bit){
                any * data_p = (any*)bit->data;
                return ((any&)*data_p);
            } return 0;
        }
        
        /*! @brief Gets *data from a variable bit.
         @param bit   The bit.
         Eg: int * p = any.get_p<int>(...);
         */
        template<typename any>
        any get_p(db_bit * bit){
            if(bit){
                return (any*)bit->data;
            } return 0;
        }
        
        /*! @brief Get dependece. */
        lan::db_bit * get_array_bit(lan::db_bits *, size_t);
        
        /*! @brief Get dependece. */
        lan::db_bit * get_last_bit(lan::db_bits *);
        
        /*! @brief Get dependece. */
        lan::db_bit_type get_var_type(std::string const);
        
        /*! @brief Returns the *data of a specific bit in the main context.
         * #Deprecated */
        void * operator [](std::string);
        
        /* Set */
        
        /*! @brief Sets a variable bit in the main context.
         @param name    The name of the bit.
         @param value   The value of the bit.
         @param type    The type of the bit.
         @param overwrite   Flag to override an existing bit with the same name and type.
         Eg: any.get<int>(...);
         */
        template<typename any>
        bool set(std::string const name, any const value, lan::db_bit_type type, bool overwrite = false){
            if(type >= lan::Array) return false;
            if((data = find_any(name, type, first))){
                if(not overwrite) throw lan::errors::overriding_bit_error(error_string(errors::_private::_overriding_bit_error, data->key));
                return set_bit(nullptr,data, name, type, value);
            } return (not first) ? init(name, value, type) : append(name, value, type);
        }
        
        /*! @brief Sets a variable bit in an array.
         @param value   The value of the bit.
         @param index   The index of the bit.
         @param type    The type of the bit.
         Eg: any.get<int>(...);
         */
        template<typename any>
        bool set(std::string array, size_t index,  any const value, lan::db_bit_type type){
            if((data = find_rec(array, lan::Container, lan::Array, first))){
                data=data->lin;
                for(register_t it = 0 ; (it < index) and data ; it++, data=data->nex);
                if(data){
                    if(data->data)
                        data->~db_bit();
                    data->type = type;
                    if(type < lan::Array)
                        data->data = new any (value);
                } 
            } throw lan::errors::bit_name_error(error_string(errors::_private::_bit_name_error, array+"{Array}"));
        }
        
        /*! @brief Sets a variable bit in a context.
         @param context The context.
         @param name    The name of the bit.
         @param value   The value of the bit.
         @param type    The type of the bit.
         @param overwrite   Flag to override an existing bit with the same name and type.
         Eg: any.get<int>(...);
         */
        template<typename any>
        bool set(std::string const context, std::string const name, any const value, lan::db_bit_type type, bool overwrite = false){
            if(type >= lan::Array) return false;
            if((data = find_rec(context, lan::Container, first))){
                lan::db_bit * buffer = data;
                if((data = find_any(name, type, data->lin))) {
                    if(!overwrite) throw lan::errors::overriding_bit_error(error_string(errors::_private::_overriding_bit_error, data->key));
                    return set_bit(buffer, data, name, type, value);
                } return (buffer->lin) ? append(buffer, name, value, type) : init(buffer, name, value, type);
            } throw lan::errors::bit_name_error(error_string(errors::_private::_bit_name_error, context+"{Container}"));;
        }
        
        /*! @brief Sets the anchor, aka "@", to a specific bit. Depending in how it's used, an anchor may potentialy speed up the program.
         @param array   The array.
         @param index   The index that we will be pointing to.
         Note: Anchors can't be variables, only Containers and Arrays are supported.
         */
        lan::anchor_t * set_anchor(std::string const array, size_t index){
            if ((data = find_rec(array, lan::Container, lan::Array, first)) and
                (anchor = get_array_bit(data, index))) return anchor;
            else throw lan::errors::anchor_name_error(error_string(errors::_private::_anchor_name_error, array+"["+std::to_string(index)+"]"));
        }
        
        /*! @brief Sets the anchor, aka "@", to a specific bit. Depending in how it's used, an anchor may potentialy speed up the program.
         @param context The context.
         Note: Anchors can't be variables, only Containers and Arrays are supported.
         */
        lan::anchor_t * set_anchor(std::string const context){
            if ((data = find_rec(context, lan::Container, first))) return (anchor = data);
            else throw lan::errors::anchor_name_error(error_string(errors::_private::_anchor_name_error, context));
        }
        
        /*! @brief Sets the anchor, aka "@", to a specific bit. Depending in how it's used, an anchor may potentialy speed up the program.
         @param anchor an bit pointer to use as anchor.
         Note: Anchors can't be variables, only Containers and Arrays are supported.
         */
        lan::anchor_t * set_anchor(lan::anchor_t * anchor){
            if ((this->anchor = anchor)) return anchor;
            else throw lan::errors::anchor_name_error(error_string(errors::_private::_anchor_name_error, "nullptr"));
        }
        
        /* Remove */
        
        /*! @brief Removes a bit.
         @param name The name of the bit.
         @param type The type of the bit.
         */
        bool remove(std::string const name, db_bit_type const type);
        
        /*! @brief Removes a bit.
         @param context The context.
         @param name The name of the bit.
         @param type The type of the bit.
         */
        bool remove(std::string const context, std::string const name, db_bit_type const type);
        
        /*! @brief Removes a bit.
         @param array The array.
         @param index The index of the bit in array.
         */
        bool remove(std::string const array, size_t index);
        
        /* -- */
        
        ~db();
    };
} /* namespace lan */
