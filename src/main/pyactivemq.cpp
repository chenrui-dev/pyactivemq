/*
  Copyright 2007 Albert Strasheim <fullung@gmail.com>

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

// TODO map clones so that they work with Python's copy module

#include <boost/python.hpp>

#include <cms/Startable.h>
#include <cms/Stoppable.h>
#include <cms/Closeable.h>
#include <cms/DeliveryMode.h>
#include <cms/CMSException.h>

using namespace boost::python;
using cms::CMSException;
using cms::Startable;
using cms::Stoppable;
using cms::Closeable;

void CMSException_translator(const CMSException& e) {
    PyErr_SetString(PyExc_UserWarning, e.getMessage().c_str());
}

template<typename T>
struct to_tuple
{
    static PyObject* convert(const T& c)
    {
        list result;
        for (typename T::const_iterator it = c.begin();
             it != c.end();
             ++it)
        {
            result.append(*it);
        }
        return incref(tuple(result).ptr());
    }
};

template<typename T>
struct std_vector_to_tuple
{
    std_vector_to_tuple()
    {
        to_python_converter<std::vector<T>,
            to_tuple<std::vector<T> > >();
    }
};

void export_ConnectionFactory();
void export_ActiveMQConnectionFactory();
void export_Connection();
void export_Session();
void export_Destinations();
void export_Message();
void export_TextMessage();
void export_BytesMessage();
void export_MapMessage();
void export_ExceptionListener();
void export_MessageProducer();
void export_MessageListener();
void export_MessageConsumer();

BOOST_PYTHON_MODULE(pyactivemq)
{
    std_vector_to_tuple<std::string>();

    class_<Startable, boost::noncopyable>("Startable", no_init)
        .def("start", &Startable::start)
        ;

    class_<Stoppable, boost::noncopyable>("Stoppable", no_init)
        .def("stop", &Stoppable::stop)
        ;

    class_<Closeable, boost::noncopyable>("Closeable", no_init)
        .def("close", &Closeable::close)
        ;

    void (CMSException::*CMSException_printStackTrace0)() const =
        &CMSException::printStackTrace;
    class_<CMSException, boost::noncopyable>("CMSException", no_init)
        .def("printStackTrace", CMSException_printStackTrace0)
        .def("getStackTraceString", &CMSException::getStackTraceString)
        .add_property("message", &CMSException::getMessage)
        ;
    register_exception_translator<CMSException>(CMSException_translator);

    export_ConnectionFactory();
    export_ActiveMQConnectionFactory();
    export_Connection();
    export_Session();
    export_Destinations();
    export_Message();
    export_TextMessage();
    export_BytesMessage();
    export_MapMessage();
    export_ExceptionListener();
    export_MessageProducer();
    export_MessageListener();
    export_MessageConsumer();

#if 1
    // TODO DeliveryMode causes problems with GCC
    scope s = class_<cms::DeliveryMode, boost::noncopyable>("DeliveryMode", no_init);
    s.attr("PERSISTENT") = cms::DeliveryMode::PERSISTENT;
    s.attr("NON_PERSISTENT") = cms::DeliveryMode::NON_PERSISTENT;
#endif
}