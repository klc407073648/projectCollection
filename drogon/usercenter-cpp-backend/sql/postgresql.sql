-- 可以使用Navicat,将MySQL转化为PostgreSQL
CREATE TABLE "user" (
  "username" varchar(256),
  "id" bigint NOT NULL,
  "userAccount" varchar(256),
  "avatarUrl" varchar(1024),
  "gender" smallint,
  "userPassword" varchar(512) NOT NULL,
  "iphone" varchar(128),
  "email" varchar(512) NOT NULL,
  "userStatus" int4 NOT NULL DEFAULT 0,,
  "createTime" timestamp NOT NULL DEFAULT now(),
  "updateTime" timestamp NOT NULL DEFAULT now(),
  "idDelete" smallint NOT NULL DEFAULT 0,
  "userRole" smallint NOT NULL DEFAULT 0,
  "planetCode" varchar(512),
  PRIMARY KEY ("id")
);
COMMENT ON COLUMN "user"."username" IS '用户昵称';
COMMENT ON COLUMN "user"."id" IS 'id';
COMMENT ON COLUMN "user"."userAccount" IS '账号';
COMMENT ON COLUMN "user"."avatarUrl" IS '用户头像';
COMMENT ON COLUMN "user"."gender" IS '性别';
COMMENT ON COLUMN "user"."userPassword" IS '密码';
COMMENT ON COLUMN "user"."iphone" IS '电话';
COMMENT ON COLUMN "user"."email" IS '邮箱';
COMMENT ON COLUMN "user"."userStatus" IS '状态-0正常';
COMMENT ON COLUMN "user"."createTime" IS '创建时间';
COMMENT ON COLUMN "user"."idDelete" IS '是否删除';
COMMENT ON COLUMN "user"."userRole" IS '用户角色 0-普通用户 1-管理员';
COMMENT ON COLUMN "user"."planetCode" IS '星球编号';