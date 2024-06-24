#version 330 core
out vec4 FragColor;
uniform int asciiChar;

void main() {
    float intensity = float(asciiChar - 32) / 95.0; // Assuming ASCII range 32-126
    FragColor = vec4(intensity, intensity, intensity, 1.0);
}
