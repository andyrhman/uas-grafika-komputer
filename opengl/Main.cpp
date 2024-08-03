//------- Abaikan ini ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------

#include"Model.h"

const unsigned int width = 1920;
const unsigned int height = 1080;

int main()
{
	// Inisialisasi GLFW
	glfwInit();

	// Beritahu GLFW versi OpenGL yang kita gunakan
	// Dalam hal ini kita menggunakan OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Beritahu GLFW kita menggunakan profil CORE
	// Jadi itu berarti kita hanya memiliki fungsi modern
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Buat objek GLFWwindow dengan ukuran 1920 x 1080 piksel
	GLFWwindow* window = glfwCreateWindow(width, height, "Grafik 3D OpenGL", NULL, NULL);
	// Periksa kesalahan jika jendela gagal dibuat
	if (window == NULL)
	{
		std::cout << "Gagal membuat jendela GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Perkenalkan jendela ke dalam konteks saat ini
	glfwMakeContextCurrent(window);

	// Muat GLAD sehingga mengkonfigurasi OpenGL
	gladLoadGL();
	// Tentukan viewport OpenGL di jendela
	// Dalam hal ini viewport berjalan dari x = 0, y = 0, hingga x = 1920, y = 1080
	glViewport(0, 0, width, height);

	// Menghasilkan objek Shader menggunakan shader default.vert dan default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// Tangani semua hal yang berhubungan dengan cahaya
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// Aktifkan Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Membuat objek kamera
	Camera camera(width, height, glm::vec3(0.0f, 8.0f, 20.0f));

	// Memuat kode texture menggunakan gltf
	Model model("models/sword/scene.gltf");

	// Loop utama
	while (!glfwWindowShouldClose(window))
	{
		// Tentukan warna latar belakang
		glClearColor(0.9f, 0.9f, 0.9f, 1.0f); // Diubah menjadi abu-abu terang
		// Bersihkan back buffer dan depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Tangani input kamera
		camera.Inputs(window);
		// Perbarui dan ekspor matriks kamera ke Vertex Shader
		camera.updateMatrix(80.0f, 0.1f, 100.0f);

		// Gambar model
		model.Draw(shaderProgram, camera);

		// Tukar back buffer dengan front buffer
		glfwSwapBuffers(window);
		// Tangani semua event GLFW
		glfwPollEvents();
	}

	// Hapus semua objek yang telah kita buat
	shaderProgram.Delete();
	// Hapus jendela sebelum mengakhiri program
	glfwDestroyWindow(window);
	// Akhiri GLFW sebelum mengakhiri program
	glfwTerminate();
	return 0;
}
