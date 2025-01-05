#pragma once

#include "glm.hpp"

namespace glimac
{

    class FreeflyCamera
    {
    public:
        FreeflyCamera() : m_Position(glm::vec3(0.f, 0.f, -5.f)), m_fPhi(0.f), m_fTheta(0.f) { computeDirectionVectors(); }

        void moveLeft(float t)
        {
            auto futurePosition = m_Position + t * m_LeftVector;
            if (futurePosition.x > -31 && futurePosition.x < 9 && futurePosition.z > -11 && futurePosition.z < 11)
            {
                if ((futurePosition.z < -1.5 || futurePosition.z > 1.5) && (futurePosition.x > -13.5 && futurePosition.x < -9.5))
                {
                    return;
                }
                m_Position += t * m_LeftVector;
            }
            m_Position.y = 0;
        }

        void moveFront(float t)
        {
            auto futurePosition = m_Position + t * m_FrontVector;
            if (futurePosition.x > -31 && futurePosition.x < 9 && futurePosition.z > -11 && futurePosition.z < 11)
            {
                if ((futurePosition.z < -1.5 || futurePosition.z > 1.5) && (futurePosition.x > -13.5 && futurePosition.x < -9.5))
                {
                    return;
                }
                m_Position += t * m_FrontVector;
            }
            m_Position.y = 0;
        }

        void rotateLeft(float degrees)
        {
            m_fPhi += glm::radians(degrees);
            computeDirectionVectors();
        }

        void rotateUp(float degrees)
        {
            m_fTheta += glm::radians(degrees);
            computeDirectionVectors();
        }

        glm::mat4 getViewMatrix() const
        {
            return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
        }

        glm::vec3 getPosition() const
        {
            return m_Position;
        }

    private:
        glm::vec3 m_Position;
        float m_fPhi;
        float m_fTheta;
        glm::vec3 m_FrontVector;
        glm::vec3 m_LeftVector;
        glm::vec3 m_UpVector;

        void computeDirectionVectors()
        {
            m_FrontVector = glm::vec3(
                glm::cos(m_fTheta) * glm::sin(m_fPhi),
                glm::sin(m_fTheta),
                glm::cos(m_fTheta) * glm::cos(m_fPhi));

            m_LeftVector = glm::vec3(
                glm::sin(m_fPhi + glm::half_pi<float>()),
                0,
                glm::cos(m_fPhi + glm::half_pi<float>()));

            m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
        }
    };
}