#include <array>
#include <cstdint>

#include "brick/platform/stm32/f1/profiles/termotechnika_st9727_480x272.h"

namespace {
constexpr std::uint16_t kWidth = 480;
constexpr std::uint16_t kHeight = 272;
constexpr std::uint16_t kStripeHeight = 34;
constexpr std::uint16_t kMarkerSize = 24;

brick::platform::stm32::f1::Ssd1963ParallelDisplay display(
    brick::platform::stm32::f1::profiles::termotechnika_st9727_480x272());
brick::platform::stm32::f1::Ft5x06Touchscreen
    touch(brick::platform::stm32::f1::profiles::termotechnika_st9727_ft5x06());

I2C_HandleTypeDef touch_i2c{};

std::array<std::uint16_t, kWidth * kStripeHeight> stripe{};
std::array<std::uint16_t, kMarkerSize * kMarkerSize> marker{};

bool draw_rows(std::uint16_t y, std::uint16_t height, std::uint16_t color) {
  for (std::size_t index = 0; index < static_cast<std::size_t>(kWidth) * height;
       ++index)
    stripe[index] = color;
  const brick::interfaces::display::PixelBuffer buffer{
      reinterpret_cast<const std::uint8_t *>(stripe.data()),
      kWidth,
      height,
      static_cast<std::size_t>(kWidth) * sizeof(std::uint16_t),
      brick::interfaces::display::PixelFormat::rgb565,
      false};
  return display.draw_buffer({0, y, kWidth, height}, buffer);
}

void draw_test_pattern() {
  constexpr std::uint16_t colors[] = {0xF800, 0x07E0, 0x001F, 0xFFE0,
                                      0xF81F, 0x07FF, 0xFFFF, 0x4208};
  for (std::uint16_t index = 0; index < 8; ++index)
    draw_rows(index * kStripeHeight, kStripeHeight, colors[index]);
}

void draw_touch_marker(const brick::interfaces::display::TouchPoint &point,
                       bool active) {
  const auto x = static_cast<std::uint16_t>(point.x < kMarkerSize / 2 ? 0
                                            : point.x > kWidth - kMarkerSize / 2
                                                ? kWidth - kMarkerSize
                                                : point.x - kMarkerSize / 2);
  const auto y = static_cast<std::uint16_t>(
      point.y < kMarkerSize / 2             ? 0
      : point.y > kHeight - kMarkerSize / 2 ? kHeight - kMarkerSize
                                            : point.y - kMarkerSize / 2);
  for (std::uint16_t row = 0; row < kMarkerSize; ++row)
    for (std::uint16_t column = 0; column < kMarkerSize; ++column) {
      const bool cross =
          (row >= 10 && row < 14) || (column >= 10 && column < 14);
      marker[static_cast<std::size_t>(row) * kMarkerSize + column] =
          active && cross ? 0x0000 : 0xFFFF;
    }
  const brick::interfaces::display::PixelBuffer buffer{
      reinterpret_cast<const std::uint8_t *>(marker.data()),
      kMarkerSize,
      kMarkerSize,
      static_cast<std::size_t>(kMarkerSize) * sizeof(std::uint16_t),
      brick::interfaces::display::PixelFormat::rgb565,
      false};
  display.draw_buffer({x, y, kMarkerSize, kMarkerSize}, buffer);
}

void configure_clock() {
  RCC_OscInitTypeDef oscillator{};
  oscillator.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  oscillator.HSEState = RCC_HSE_ON;
  oscillator.HSIState = RCC_HSI_ON;
  oscillator.PLL.PLLState = RCC_PLL_ON;
  oscillator.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  oscillator.PLL.PLLMUL = RCC_PLL_MUL9;
  HAL_RCC_OscConfig(&oscillator);
  RCC_ClkInitTypeDef clock{};
  clock.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                    RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  clock.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clock.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clock.APB1CLKDivider = RCC_HCLK_DIV2;
  clock.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&clock, FLASH_LATENCY_2);
}

bool configure_touch_i2c() {
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_I2C1_CLK_ENABLE();
  GPIO_InitTypeDef pins{};
  pins.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  pins.Mode = GPIO_MODE_AF_OD;
  pins.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &pins);
  touch_i2c.Instance = I2C1;
  touch_i2c.Init.ClockSpeed = 400000;
  touch_i2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
  touch_i2c.Init.OwnAddress1 = 0;
  touch_i2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  touch_i2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  touch_i2c.Init.OwnAddress2 = 0;
  touch_i2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  touch_i2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  return HAL_I2C_Init(&touch_i2c) == HAL_OK;
}
} // namespace

int main() {
  HAL_Init();
  configure_clock();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  auto touch_config =
      brick::platform::stm32::f1::profiles::termotechnika_st9727_ft5x06();
  touch_config.i2c = &touch_i2c;
  touch = brick::platform::stm32::f1::Ft5x06Touchscreen(touch_config);
  if (!configure_touch_i2c() || !display.begin() || !touch.begin())
    while (true)
      HAL_Delay(1000);
  draw_test_pattern();
  while (true) {
    std::array<brick::interfaces::display::TouchPoint, 1> points{};
    std::size_t count = 0;
    if (touch.read(points.data(), points.size(), count) && count != 0 &&
        points.front().state !=
            brick::interfaces::display::TouchState::released)
      draw_touch_marker(points.front(), true);
    HAL_Delay(20);
  }
}
