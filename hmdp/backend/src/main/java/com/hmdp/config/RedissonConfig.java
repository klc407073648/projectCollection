package com.hmdp.config;

import lombok.Data;
import org.redisson.Redisson;
import org.redisson.api.RedissonClient;
import org.redisson.config.Config;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
@ConfigurationProperties(prefix = "spring.redis")
@Data
public class RedissonConfig {
    private String host;
    private String port;

    private String password;
    /**
     * 自定义Bean
     * 打上Bean注解，Spring Boot,会自动将RedissonClient对象管理起来，其他地方可以通过@Resource来使用
     */
    @Bean
    public RedissonClient redissonClient(){
        // 1. 创建配置
        Config config = new Config();
        String redisAddress = String.format("redis://%s:%s",host,port);
        config.useSingleServer().setAddress(redisAddress).setPassword(password);
        //2. 创建 Redisson 实例
        RedissonClient redisson = Redisson.create(config);

        return redisson;
    }
}
